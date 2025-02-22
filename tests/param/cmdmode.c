// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "partest.h"

#include "param.h"

const char *usage[] = {
	"run [--proc | --thrd]",
	NULL,
	NULL,
};

enum op_mode {
	OP_PROC,
	OP_THRD,
};

int __main(int argc, const xchar **argv)
{
	enum op_mode mode = OP_PROC;

	struct opt opt[] = {
		OPT_CMDMODE(0, "proc", &mode, OP_PROC, "process"),
		OPT_CMDMODE(0, "thrd", &mode, OP_THRD, "thread"),
		OPT_END(),
	};

	param_parse(argc, argv, usage, opt, 0);

	if (mode == OP_PROC)
		pprint_cmdmode(OP_PROC);
	else if (mode == OP_THRD)
		pprint_cmdmode(OP_THRD);
	return 0;
}
