// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "partest.h"

#include "param.h"

const char *usage[] = {
	"run [-f | --[no-]force] [--[no-]enable]",
	NULL,
	NULL,
};

int __main(int argc, const xchar **argv)
{
	int force = 0;
	int enable = 0;

	struct opt opt[] = {
		OPT_SWITCH('f', "force", &force, "forced to do something"),
		OPT_SWITCH(0, "enable", &enable, "enable something"),
		OPT_END(),
	};

	param_parse(argc, argv, usage, opt, 0);

	pprint_int(force);
	pprint_int(enable);
	return 0;
}
