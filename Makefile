# SPDX-License-Identifier: GPL-3.0-or-later

ifneq ($(filter extra-prereqs,$(.FEATURES)),extra-prereqs)
$(error GNU Make >= 4.3 is required. Your Make version is $(MAKE_VERSION))
endif

MAKEFLAGS += -rR
MAKEFLAGS += --no-print-directory

srctree := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
gendir  := $(srctree)/include/generated
objtree := $(srctree)/build.unix

generator := Ninja

export SRCTREE := $(srctree)
export GENDIR  := $(gendir)
export OBJTREE := $(objtree)

export CC := gcc
export LD := ld.bfd

ifneq ($(LLVM),)
export CC := clang
export LD := ld.lld
endif

ifneq ($(MAKEFILE),)
generator := Unix Makefiles
endif

export LASTPLAT := $(srctree)/.lastplat

export DOTCONFIG := $(srctree)/.config.unix
export DEFCONFIG := $(DOTCONFIG).def

ifneq ($(wildcard $(DOTCONFIG)),)
RELCONFIG := $(DOTCONFIG)
else
RELCONFIG    := $(DEFCONFIG)
MK_DEFCONFIG := mk_defconfig
endif

export RELCONFIG
export KCONFIG_CONFIG := $(RELCONFIG)

ifneq ($(wildcard $(DOTCONFIG)),)
ifneq ($(wildcard $(DEFCONFIG)),)
RECONFIGURE  := configure
RM_DEFCONFIG := rm_defconfig
else
RERECONFDEP  := reconfdep
endif
endif

export RECONFDEP := $(objtree)/reconfdep

CMAKE_CC_FEATURE := $(objtree)/features.cmake

build:

.PHONY: menuconfig mk_defconfig rm_defconfig \
	reconfdep configure lastplat build all

menuconfig:
	@scripts/kconfig.py menuconfig

$(gendir):
	@mkdir $@

$(CMAKE_CC_FEATURE): $(gendir)
	@scripts/cc-feature.py cmake

mk_defconfig:
	@scripts/kconfig.py alldefconfig

rm_defconfig:
	@rm $(DEFCONFIG)

reconfdep: $(MK_DEFCONFIG) $(RM_DEFCONFIG)
	@scripts/reconfdep.py $(RELCONFIG) $(RECONFDEP)

configure: $(CMAKE_CC_FEATURE) reconfdep
	@cmake -G "$(generator)" -S . -B $(objtree) $(EXTOPT)

lastplat:
	@if [ -f $(objtree)/CMakeCache.txt ] &&			\
	    [ "$$(cat $(LASTPLAT) 2>&1 )" != unix ]; then	\
		echo unix > $(LASTPLAT);			\
	fi

build: lastplat $(RECONFIGURE) $(RERECONFDEP)
	@cmake --build $(objtree) --parallel

all: configure build

.PHONY: clean distclean

clean:
	@cmake --build $(objtree) --target clean

distclean:
	@rm -rf include/generated
	@rm -f include/arch
	@rm -f $(DOTCONFIG)*
	@rm -f $(LASTPLAT)
	@git ls-files --directory -o $(objtree) | xargs rm -rf

__tests := $(wildcard $(objtree)/t/*.t)
tests   := $(patsubst $(objtree)/%,%,$(__tests))

.PHONY: $(tests)

$(tests):
	@$(objtree)/$@

.PHONY: t/all

t/all:
	@ctest --test-dir $(objtree)/tests --parallel $(shell nproc)

scripts := $(wildcard scripts/*.sh) $(wildcard scripts/*.py)
args    := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

.PHONY: $(args)

$(args):
	@:

.PHONY: $(scripts)

$(scripts):
	@./$@ $(args)
