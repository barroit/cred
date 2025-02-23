// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "partest.h"

#include "param.h"

const char *usage[] = {
	"run [--name=<name>]",
	NULL,
	NULL,
};

int __main(int argc, const xchar **argv)
{
	const xchar *name = NULL;

	struct opt opt[] = {
		OPT_STRING(0, "name", &name, "someone's name"),
		OPT_END(),
	};

	param_parse(argc, argv, usage, opt, 0);

	pprint_str(name);
	return 0;
}
