// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "unitest.h"

#include "fiter.h"

#include <unistd.h>

#include "iter.h"
#include "path.h"

#if defined(_WIN32) && defined(CONFIG_WIDE_CHAR)
# define access _waccess
#endif

TESTDECL_BEGIN();

static void __cb(struct file *f, void *data)
{
	int err = access(f->path, F_OK);

	USSERT_NOT(err);
}

static int cb(struct file *f, void *data)
{
	uint cnt = __test_failure_count;

	__cb(f, data);
	if (__test_failure_count != cnt)
		return -1;

	return 0;
}

TESTDECL_ROUTINE(fiter)
{
	const xchar *ws = pth_prefix();
	u32 flags[] = {
		FI_LIST_DIR,
		FI__LIST_DIR_ONLY,
		FI_RECUR_DIR,
		FI__RECUR_DIR_ONLY,
		FI_NO_LNK,
		FI_NO_REG,
		FI_NO_UNK,
		FI_USE_STAT,
	};
	uint i;

	idx_for_each(i, sizeof_array(flags)) {
		int err = fiter(ws, cb, NULL, FI_RECUR_DIR);

		USSERT_NOT(err);
	}
}

TESTDECL_END();
