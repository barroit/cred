# SPDX-License-Identifier: GPL-3.0-or-later or MIT

run --kdf scrypt
PSSERT_OPTEQUAL kdf scrypt

run --kdf scrypt --kdf=argon2
PSSERT_OPTEQUAL kdf argon2

run --kdf fake_kdf
PSSERT_ERRLIKE valid choices are
