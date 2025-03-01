# SPDX-License-Identifier: GPL-3.0-or-later

ifneq ($(filter extra-prereqs,$(.FEATURES)),extra-prereqs)
$(error GNU Make >= 4.3 is required. Your Make version is $(MAKE_VERSION))
endif

MAKEFLAGS += -rR
MAKEFLAGS += --no-print-directory

export TOP   := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
export GEN   := $(TOP)/include/generated
export BUILD := $(TOP)/build.unix

ifneq ($(LLVM),)
CC := clang
LD := ld.lld
else
CC := gcc
LD := ld.bfd
endif

export CC LD

export LASTPLAT := $(TOP)/.lastplat

export DOTCONFIG := $(TOP)/.config.unix
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

export RECONFDEP := $(BUILD)/reconfdep

CMAKE_CC_FEATURE := $(BUILD)/features.cmake

build:

.PHONY: menuconfig mk_defconfig rm_defconfig \
	reconfdep configure lastplat build all

menuconfig:
	@scripts/kconfig.py menuconfig

$(GEN):
	@mkdir $@

$(CMAKE_CC_FEATURE): $(GEN)
	@scripts/cc-feature.py cmake

mk_defconfig:
	@scripts/kconfig.py alldefconfig

rm_defconfig:
	@rm $(DEFCONFIG)

reconfdep: $(MK_DEFCONFIG) $(RM_DEFCONFIG)
	@scripts/reconfdep.py $(RELCONFIG) $(RECONFDEP)

configure: $(CMAKE_CC_FEATURE) reconfdep
	@cmake -S . -B $(BUILD) $(EXTOPT)

lastplat:
	@if [ -f $(BUILD)/CMakeCache.txt ] &&			\
	    [ "$$(cat $(LASTPLAT) 2>&1 )" != unix ]; then	\
		echo unix > $(LASTPLAT);			\
	fi

build: lastplat $(RECONFIGURE) $(RERECONFDEP)
	@cmake --build $(BUILD) --parallel

all: configure build

.PHONY: clean distclean

clean:
	@cmake --build $(BUILD) --target clean

distclean:
	@rm -rf include/generated
	@rm -f include/arch
	@rm -f $(DOTCONFIG)*
	@rm -f $(LASTPLAT)
	@git ls-files --directory -o $(BUILD) | xargs rm -rf

__tests := $(wildcard $(BUILD)/t/*.t)
tests   := $(patsubst $(BUILD)/%,%,$(__tests))

.PHONY: $(tests)

$(tests):
	@./$(BUILD)/$@

.PHONY: t/all

t/all:
	@ctest --test-dir $(BUILD)/tests --parallel $(shell nproc)

scripts := $(wildcard scripts/*.sh) $(wildcard scripts/*.py)
args    := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

.PHONY: $(args)

$(args):
	@:

.PHONY: $(scripts)

$(scripts):
	@./$@ $(args)
