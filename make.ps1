# SPDX-License-Identifier: GPL-3.0-or-later
# Using PowerShell feels like shoving a cactus up your ass. This is what
# you get when you let a bunch of clueless idiots design a language. It's
# a fucking nightmare dressed as a tool.

#requires -version 6

param
(
	[ArgumentCompletions('menuconfig', 'configure', 'build', 'all',`
			     'clean', 'distclean', 'install', 'uninstall')]
	[string]$__target = 'build'
)

$ErrorActionPreference = 'Stop'

Set-Alias error Write-Error
Set-Alias rm_f  Remove-Item -Force -ErrorAction SilentlyContinue

$0 = $MyInvocation.MyCommand.Path

function export
{
	param([string]$str)

	$pair  = $str -split '=', 2
	$name  = $pair[0]
	$value = $pair[1]

	[System.Environment]::SetEnvironmentVariable($name, $value, 'Process')
	Set-Variable -Scope Script $name $value
}

$BUILD_NAME = 'build.win32'

export TOP=$($PSScriptRoot.Replace('\','/'))
# $TOP = $TOP.TrimEnd('/')

export GEN=$TOP/include/generated
export BUILD=$TOP/$BUILD_NAME

export CC=clang.exe
export LD=ld.lld.exe

export LASTPLAT=$TOP/.lastplat

export DOTCONFIG=$TOP/.config.win32
export DEFCONFIG=$DOTCONFIG.def

if (Test-Path $DOTCONFIG) {
	$RELCONFIG    = $DOTCONFIG
} else {
	$RELCONFIG    = $DEFCONFIG
	$MK_DEFCONFIG = 1
}

export RELCONFIG=$RELCONFIG
export KCONFIG_CONFIG=$RELCONFIG

if (Test-Path $DOTCONFIG) {
	if (Test-Path $DEFCONFIG) {
		$RECONFIGURE  = 1
		$RM_DEFCONFIG = 1
	} else {
		$RERECONFDEP  = 1
	}
}

export RECONFDEP=$BUILD/reconfdep

$__menuconfig = 1 -shl 0
$__configure  = 1 -shl 1
$__build      = 1 -shl 2
$__all	      = $__configure -bor $__build
$__clean      = 1 -shl 3
$__distclean  = 1 -shl 4
$__install    = 1 -shl 5
$__uninstall  = 1 -shl 6
$__test       = 1 -shl 7
$__reconfdep  = 1 -shl 8

switch -CaseSensitive ($__target) {
'menuconfig' { $target = $__menuconfig; break }
'reconfdep'  { $target = $__reconfdep;  break }
'configure'  { $target = $__configure;  break }
'build'	     { $target = $__build;      break }
'all'	     { $target = $__all;        break }
'clean'	     { $target = $__clean;      break }
'distclean'  { $target = $__distclean;  break }
'install'    { $target = $__install;    break }
'uninstall'  { $target = $__uninstall;  break }
}

if (!$target) {
	switch -Wildcard ($__target) {
	't/*'       { $target = $__test; break}
	}

	if (!$target) {
		error "unknown target '$__target'"
	}
}

if ($target -band $__menuconfig) {
	python scripts/kconfig.py menuconfig
}

if ($target -band $__reconfdep) {
	if ($MK_DEFCONFIG) {
		python scripts/kconfig.py alldefconfig
	}

	if ($RM_DEFCONFIG) {
		rm $DEFCONFIG
	}

	python scripts/reconfdep.py $RELCONFIG $RECONFDEP
}

if ($target -band $__configure) {
	if (!(Test-Path $BUILD/features.cmake)) {
		if (!(Test-Path $GEN)) {
			mkdir $GEN
		}

		python scripts/cc-feature.py cmake
	}

	& $0 reconfdep
	cmake -G Ninja -S . -B $BUILD
}

if ($target -band $__build) {
	if (!(Test-Path $LASTPLAT) -or (cat $LASTPLAT) -ne 'win32') {
		echo win32 > $LASTPLAT
	}

	if ($RECONFIGURE) {
		& $0 configure
	}

	if ($RERECONFDEP) {
		& $0 reconfdep
	}

	cmake --build $BUILD --parallel
}

if ($target -band $__clean) {
	cmake --build $BUILD --target clean
}

if ($target -band $__distclean) {
	$dotconfig = Get-ChildItem -Force $env:KCONFIG_CONFIG*
	$buildgens = git ls-files --directory -o $BUILD_NAME

	if (Test-Path include/generated) {
		Remove-Item -Recurse include/generated
	}
	if ($dotconfig) {
		Remove-Item $dotconfig
	}
	if (Test-Path $LASTPLAT) {
		Remove-Item $LASTPLAT
	}
	if (Test-Path *.manifest) {
		Remove-Item *.manifest
	}
	if ($buildgens) {
		Remove-Item -Recurse $buildgens
	}
}

$cpus = $env:NUMBER_OF_PROCESSORS

if ($target -band $__test) {
	if ($__target -eq 't/all') {
		ctest --test-dir $BUILD/tests --parallel $cpus
	} else {
		$t = "$BUILD/$__target.exe"

		if (!(Test-Path $t)) {
			error "not a test '$t'"
		}

		& $t
	}
}
