// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "partest.h"

#include "param.h"

static const char *usage[] = {
	"run push",
	"run pop",
	NULL,
	NULL,
};

static int push(int argc, const xchar **argv)
{
	return 0;
}

static int pop(int argc, const xchar **argv)
{
	return 0;
}

int __main(int argc, const xchar **argv)
{
	cmd_cb cmd = NULL;

	struct opt opt[] = {
		OPT_COMMAND("push", &cmd, push),
		OPT_COMMAND("pop", &cmd, pop),
		OPT_END(),
	};

	argc = param_parse(argc, argv, usage, opt, 0);

	if (cmd == push)
		pprint_cmd(push);
	else if (cmd == pop)
		pprint_cmd(pop);
	else
		pprint_cmd(null);

	return 0;
}
