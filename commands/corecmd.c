// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

#include "corecmd.h"

#include "param.h"
#include "proc.h"
#include "string.h"
#include "termas.h"
#include "udef.h"
#include "xcf.h"

static const char *usage[] = {
	PROGRAM_NAME " [--termas=<path> | --no-termas] [--no-tercol]"
		     " [--show-ts] [--show-pid] [--cred=<path>]"
		     " [--key=<path>] <command> [<args>]",
	NULL,
N_(
"These are common " PROGRAM_NAME " commands:"
), N_(
/*
 * TRANSLATORS
 * Do not modify the prefix '   version   '. Only translate the description of
 * the command.
 */
"   version   display version information about " PROGRAM_NAME
),
	NULL,
};

static cmd_cb subcmd;

static struct opt opts[] = {
	OPT_GROUP(N_("Format options:")),
	OPT_SWITCH(0, "tercol", &udef_use_tercol,
		   N_("enable colorized terminal output")),
	__OPT_SWITCH(0, "show-ts", &udef_termas_ts,
		     N_("prefix messages with timestamp"), OPT_NO_NEG),
	__OPT_SWITCH(0, "show-pid", &udef_termas_pid,
		     N_("prefix messages with current PID"), OPT_NO_NEG),

	OPT_GROUP(N_("Crucial file options:")),
	__OPT_FILENAME(0, "termas", &udef_termas_dest,
		       N_("specify output destination"), 0),
	OPT_FILENAME(0, "prefix", &udef_prefix, N_("specify cred prefix")),
	OPT_FILENAME(0, "cred", &udef_cred, N_("specify cred file")),
	OPT_FILENAME(0, "cred-key", &udef_cred_key, N_("specify key file")),

	OPT_COMMAND("version", &subcmd, cmd_version),
	OPT_END(),
};

int cmd_main(int argc, const xchar **argv)
{
	argc = param_parse(argc, argv, usage, opts, PRM_PARSE_CMD);

	if (udef_termas_dest != UDEF_DEF_PTR) {
		const xchar *out = udef_termas_dest ? : XC(CONFIG_NULL_DEVICE);
		int err = proc_redir_std(out, PROC_REDIR_OUT | PROC_REDIR_ERR);

		if (err)
			warn(_("failed to run process in silent mode"));
	}

	if (UDEF_DEFINED(udef_cred) &&
	    UDEF_DEFINED(udef_cred_key) &&
	    xc_strcmp(udef_cred, udef_cred_key) == 0)
		die(_("--cred and --key point to the same file `%s'"),
		    udef_cred);

	return subcmd(argc, argv);
}
