// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "partest.h"

#include "param.h"

static const char *usage[] = {
	"run [--kdf=<algo>]",
	NULL,
	NULL,
};

static const xchar *kdfs[] = {
	XC("scrypt"),
	XC("argon2"),
	NULL,
};

static const xchar *kdf;

struct opt option[] = {
	OPT_CHOICE(0, "kdf", &kdf, kdfs, "key derivation functions"),
	OPT_END(),
};


int __main(int argc, const xchar **argv)
{
	argc = param_parse(argc, argv, usage, option, 0);

	pprint_str(kdf);
	return 0;
}
