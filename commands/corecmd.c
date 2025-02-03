// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

#include "corecmd.h"

#include "param.h"
#include "proc.h"
#include "termas.h"
#include "udef.h"

static const char *usage[] = {
	PROGRAM_NAME " [--quiet] [--no-tercol] [--show-ts] [--show-pid]"
		     " <command> [<args>]",
	NULL,
};

const xchar *udef_termas_dest = UDEF_DEF_PTR;
int udef_use_tercol = 1;
int udef_termas_ts;
int udef_termas_pid;

static cmd_cb subcmd;

static struct opt opts[] = {
	__OPT_STRING(0, "output", &udef_termas_dest,
		     N_("output destination"), 0),
	OPT_SWITCH(0, "tercol", &udef_use_tercol,
		   N_("enable colorized terminal output")),
	__OPT_SWITCH(0, "show-ts", &udef_termas_ts,
		     N_("prefix messages with timestamp"), OPT_NO_NEG),
	__OPT_SWITCH(0, "show-pid", &udef_termas_pid,
		     N_("prefix messages with current PID"), OPT_NO_NEG),
	OPT_COMMAND("version", &subcmd, cmd_version),
	OPT_END(),
};

int cmd_main(int argc, const xchar **argv)
{
	argc = parse_param(argc, argv, usage, opts, PRM_PAR_CMD);

	if (udef_termas_dest != UDEF_DEF_PTR) {
		const xchar *dest = udef_termas_dest ? : CONFIG_NULL_DEVICE;
		int err = proc_redir_std(dest,
					 PROC_REDIR_OUT | PROC_REDIR_ERR);

		if (err)
			warn(_("failed to run process in silent mode"));
	}

	return subcmd(argc, argv);
}
