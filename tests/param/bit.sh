# SPDX-License-Identifier: GPL-3.0-or-later or MIT

run --all
PSSERT_OPTEQUAL show 0f

run --all --no-lib
PSSERT_OPTEQUAL show 07

run --all --no-lib --no-cflag
PSSERT_OPTEQUAL show 03

run --all --no-lib --no-cflag --no-prefix --no-build
PSSERT_OPTEQUAL show 00

run --all --no-lib --no-cflag --prefix --build
PSSERT_OPTEQUAL show 03

run --prefix --build
PSSERT_OPTEQUAL show 03
