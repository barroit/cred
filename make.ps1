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

$tree = $PSScriptRoot
Set-Alias error Write-Error

$BUILD = 'build.win32'

if ($tree -ne $PWD) {
	error "you need to be inside the source tree ($tree)"
}

$env:TREE           = $tree.Replace('\','/')
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
		    -Path *.manifest

	$f = git ls-files --directory -o $BUILD
	if ($f) {
		Remove-Item -Recurse -Force -ErrorAction SilentlyContinue `
			    -Path (git ls-files --directory -o $BUILD)
	}
} elseif ($target -eq 'menuconfig') {
	python scripts/kconfig.py menuconfig
}
