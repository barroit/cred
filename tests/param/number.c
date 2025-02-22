// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "partest.h"

#include "param.h"

const char *usage[] = {
	"run [--job=<N>]",
	NULL,
	NULL,
};

int __main(int argc, const xchar **argv)
{
	uint job = 0;

	struct opt opt[] = {
		OPT_NUMBER('j', "job", &job, "allow N jobs at once"),
		OPT_END(),
	};

	param_parse(argc, argv, usage, opt, 0);

	pprint_uint(job);
	return 0;
}
