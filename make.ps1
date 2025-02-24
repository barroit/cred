# SPDX-License-Identifier: GPL-3.0-or-later or MIT
# Using PowerShell feels like shoving a cactus up your ass. This is what
# you get when you let a bunch of clueless idiots design a language. It's
# a fucking nightmare dressed as a tool.

#requires -version 6

param
(
	[ArgumentCompletions('configure', 'build', `
			     'all', 'test', 'clean', 'distclean', `
			     'menuconfig', 'install', 'uninstall')]
	[string]$first
)

Set-Alias error Write-Error

$TOP   = $PSScriptRoot.Replace('\','/')
$GEN   = "$TOP/include/generated"
$BUILD = "$TOP/build.win32"

$env:TOP   = $TOP
$env:GEN   = $GEN
$env:BUILD = $BUILD

$env:KCONFIG_CONFIG = '.config.win32'

$targets = @('configure', 'build', 'all', 'test', 'clean', `
	     'distclean', 'menuconfig', 'install', 'uninstall')

if (! $first) {
	# make
	$target = 'build'
} elseif ($first -in $targets) {
	# make all
	$target = $first
} else {
	# make LLVM=1
	# or
	# make LLVM=1 configure
	# or
	# make xxxx
	$args = @($first) + $args
	$target = 'build'
	if ($args[-1] -in $targets) {
		$target = $args[-1]
	}
}

if ($args | Where-Object { $_ -like 'GMAKE=*' }) {
	$GMAKE = 1
}

$env:CC = 'clang'
# Append an extension to make python correctly find that executable
$env:LD = 'ld.lld.exe'

if ($target -eq 'configure' -or $target -eq 'all') {
	if ($GMAKE) {
		$generator = 'Unix Makefiles'
	} else {
		$generator = 'Ninja'
	}

	cmake -G $generator -S . -B $BUILD
}

if ($target -eq 'build' -or $target -eq 'all') {
	if (!(Test-Path .last_build)) {
		echo win32 > .last_build
	} elseif ((cat .last_build) -ne 'win32') {
		echo win32 > .last_build
	}

	cmake --build $BUILD --parallel
}

if ($target -eq 'test') {
	ctest --test-dir $BUILD/tests --parallel $env:NUMBER_OF_PROCESSORS
} elseif ($target -eq 'clean') {
	cmake --build $BUILD --target clean
} elseif ($target -eq 'distclean') {
	Remove-Item -Recurse -Force -ErrorAction SilentlyContinue`
		    -Path include/generated

	Remove-Item -Force -ErrorAction SilentlyContinue `
		    -Path $env:KCONFIG_CONFIG*

	Remove-Item -Force -ErrorAction SilentlyContinue `
		    -Path .last_build

	Remove-Item -Force -ErrorAction SilentlyContinue `
		    -Path *.manifest

	$f = git ls-files --directory -o $BUILD
	if ($f) {
		Remove-Item -Recurse -Force -ErrorAction SilentlyContinue `
			    -Path (git ls-files --directory -o $BUILD)
	}
} elseif ($target -eq 'menuconfig') {
	python scripts/kconfig.py menuconfig
}
