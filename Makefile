# SPDX-License-Identifier: GPL-3.0-or-later or MIT

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
RELCONFIG := $(DEFCONFIG)
endif

export RELCONFIG
export KCONFIG_CONFIG := $(RELCONFIG)

ifneq ($(wildcard $(DOTCONFIG)),)
ifneq ($(wildcard $(DEFCONFIG)),)
RECONFIGURE  := configure
RM_DEFCONFIG := rm_defconifg
endif
endif

build:

.PHONY: menuconfig rm_defconifg configure lastplat build all

menuconfig:
	@scripts/kconfig.py menuconfig

$(GEN):
	@mkdir $@

$(BUILD)/features.cmake: $(GEN)
	@scripts/cc-feature.py cmake

$(RELCONFIG):
	@scripts/kconfig.py alldefconfig

rm_defconifg:
	@rm $(DEFCONFIG)

configure: $(BUILD)/features.cmake $(RELCONFIG) $(RM_DEFCONFIG)
	@$(REMCONFIG)
	@cmake -S . -B $(BUILD) $(EXTOPT)

lastplat:
	@if [ -f $(BUILD)/CMakeCache.txt ] &&			\
	    [ "$$(cat $(LASTPLAT) 2>&1 )" != unix ]; then	\
		echo unix > $(LASTPLAT);			\
	fi

build: lastplat $(RECONFIGURE)
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
