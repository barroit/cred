// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#define NG39_NO_STD_HDR_PATCH
# include <openssl/evp.h>
#undef NG39_NO_STD_HDR_PATCH

#include <sqlite3.h>
#include <stdio.h>

#include <stdlib.h>

#include "corecmd.h"
#include "gettext.h"
#include "iter.h"
#include "param.h"
#include "path.h"
#include "strbuf.h"
#include "termas.h"

static const uint limit = 1;
static const char *usage[] = {
	"init [-e | --encrypt] [--prefix=<path>] [<password>]",
	NULL,
	NULL,
};

static int encrypt;
static const xchar *prefix;

static struct opt opts[] = {
	OPT_SWITCH('e', "encrypt", &encrypt, N_("encrypt credential file")),
	OPT_FILENAME('o', "prefix", &prefix, N_("specify output *directory*")),
	OPT_END(),
};

/* sqliteInt.h */
#define SQLITE_OPEN_BINARY_KEY       0x10000000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_PASSPHRASE       0x20000000  /* Ok for sqlite3_open_v2() */

int cmd_init(int argc, const xchar **argv)
{
	u32 flags = SQLITE_OPEN_READWRITE |
		    SQLITE_OPEN_CREATE |
		    SQLITE_OPEN_EXCLUSIVE;
	const xchar *passwd = NULL;

	param_set_ex(PRM_LIM_ARG, &limit);
	argc = param_parse(argc, argv, usage, opts, PRM_LIM_ARG);

	if (argc)
		passwd = argv[0];

	if (encrypt) {
		if (!passwd)
			flags |= SQLITE_OPEN_BINARY_KEY;
		else
			flags |= SQLITE_OPEN_PASSPHRASE;
	} else if (passwd) {
		warn(_("password is ignored without --encrypt"));
		passwd = NULL;
	}

	/*
	 * TODO
	 * - Populate db filename.
	 * - Convert wchar_t filename to char.
	 */

	// if (!prefix) {
	// 	struct strbuf sb;
	// 	const xchar *home = pth_home();

	// 	sb_init_ws(&sb, home);
	// 	sb_pth_append();
	// }

	puts(sqlite3_libversion());
	puts(OpenSSL_version(OPENSSL_VERSION));

	exit(0);
}
