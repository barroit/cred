# SPDX-License-Identifier: GPL-3.0-or-later or MIT

ifneq ($(filter extra-prereqs,$(.FEATURES)),extra-prereqs)
$(error GNU Make >= 4.3 is required. Your Make version is $(MAKE_VERSION))
endif

MAKEFLAGS += -rR
MAKEFLAGS += --no-print-directory

export TOP   := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
export GEN   := $(TOP)/include/generated
export BUILD := $(TOP)/build.unix

export KCONFIG_CONFIG := .config.unix

ifneq ($(LLVM),)
CC := clang
LD := ld.lld
else
CC := gcc
LD := ld.bfd
endif

export CC LD

FEATURES_CMAKE := $(BUILD)/features.cmake

build:

.PHONY: configure build all last_build

$(GEN):
	@mkdir $@

$(FEATURES_CMAKE): $(GEN)
	@scripts/cc-feature.py cmake

last_build:
	@if [ ! -f .last_build ]; then			\
		echo unix > .last_build;		\
	elif [ "$$(cat .last_build)" != unix ]; then	\
		echo unix > .last_build;		\
	fi

build: last_build
	@cmake --build $(BUILD) --parallel

configure: $(FEATURES_CMAKE)
	@cmake -S . -B $(BUILD) $(EXTOPT)

all: configure build

.PHONY: clean distclean

clean:
	@cmake --build $(BUILD) --target clean

distclean:
	@rm -rf include/generated
	@rm -f include/arch
	@rm -f $(KCONFIG_CONFIG)*
	@rm -f .last_build
	@git ls-files --directory -o $(BUILD) | xargs rm -rf

.PHONY: menuconfig

menuconfig:
	@scripts/kconfig.py menuconfig

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
