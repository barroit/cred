/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_UDEF_H
#define NG39_UDEF_H

#include "types.h"

#define UDEF_DEF_NUM (-39)
#define UDEF_DEF_PTR ((void *)-39)

extern const xchar *udef_termas_dest;
				/* output destination */

extern int udef_use_tercol;	/* format output with colors */

extern int udef_termas_ts;	/* prefix message with timestamp */

extern int udef_termas_pid;	/* prefix message with process id */

extern const xchar *udef_prefix;
				/* cred prefix path */

extern const xchar *udef_cred;	/* cred file path */

extern const xchar *udef_cred_key;	/* key file path */

extern int udef_no_console;	/* do not allocate console */

#endif /* NG39_UDEF_H */
