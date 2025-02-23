/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_PARAM_H
#define NG39_PARAM_H

#include "attr.h"
#include "compiler.h"
#include "gettext.h"
#include "types.h"
#include "xc.h"

enum opt_mode {
	OPTION_BIT,
	OPTION_SWITCH,
	OPTION_NUMBER,
	OPTION_STRING,
	OPTION_CMDMODE,

	__OPTION__SIZE,

	OPTION__END,
	OPTION__GROUP,
	OPTION__COMMAND,
};

#define OPT_NO_ARG  (1 << 0)
#define OPT_OPT_ARG (1 << 1)
#define OPT_NO_NEG  (1 << 2)

typedef int (*cmd_cb)(int, const xchar **);

struct opt {
	enum opt_mode mode;

	xchar snam;
	const xchar *lnam;
	const char *__lnam;

	void *ptr;
	union {
		intptr_t val;
		cmd_cb cmd;
	};

	const char *argh;
	const char *usage;

	u32 flags;
};

/*
 * Flags that do not use ex[]:
 * +++++++++++
 * PRM_RET_ARG
 *	Stop parsing on encountering a non-option argument.
 *
 * PRM_PAR_CMD
 *	Stop parsing on encountering a command.
 *
 * PRM_NO_HELP
 *	Disable built-in help (-h and --help).
 *
 * PRM_OPT_CMD
 *	Make the command optional.
 *
 * PRM_NO_ARG
 *	Abort on encountering an argument.
 */
#define PRM_RET_ARG  (1 << 0)
#define PRM_PAR_CMD  (1 << 1)
#define PRM_NO_HELP  (1 << 2)
#define PRM_OPT_CMD  (1 << 3)
#define PRM_NO_ARG   (1 << 4)

/*
 * Flags that use ex[]:
 * +++++++++++
 * PRM_LIM_ARG
 *	Abort if the number of arguments exceeds the limit specified in ex[].
 */
#define PRM_LIM_ARG  (1 << 16)

#define opt_for_each(pos, opts) \
	for (pos = opts; (pos)->mode != OPTION__END; (pos)++)

void *param_set_ex(u32 flag, const void *val);

int param_parse(int argc, const xchar **argv,
		const char **usage, struct opt *opts, u32 flags);

void __noreturn param_show_help(const char **usage,
				struct opt *opts, int is_err);

#define __opt_snam(s) __st_isascii(s) + s
#define __opt_lnam(l) XC(l)
#define __opt_ptr(p, t) __st_same_type(p, t *) + p

#define OPT_END()    { .mode = OPTION__END }
#define OPT_GROUP(l) { .mode = OPTION__GROUP, .__lnam = l }

#define OPT_BIT(s, l, p, v, h) __OPT_BIT(s, l, p, v, h, 0)
#define __OPT_BIT(s, l, p, v, h, f) {	\
	.mode   = OPTION_BIT,		\
	.snam   = __opt_snam(s),	\
	.lnam   = __opt_lnam(l),	\
	.__lnam = l,			\
	.ptr    = __opt_ptr(p, u32),	\
	.val    = v,			\
	.usage  = h,			\
	.flags  = OPT_NO_ARG | (f),	\
}

#define OPT_SWITCH(s, l, p, h) __OPT_SWITCH(s, l, p, h, 0)
#define __OPT_SWITCH(s, l, p, h, f) {	\
	.mode   = OPTION_SWITCH,	\
	.snam   = __opt_snam(s),	\
	.lnam   = __opt_lnam(l),	\
	.__lnam = l,			\
	.ptr    = __opt_ptr(p, int),	\
	.usage  = h,			\
	.flags  = OPT_NO_ARG | (f),	\
}

#define OPT_NUMBER(s, l, p, h) __OPT_NUMBER(s, l, p, h, 0)
#define __OPT_NUMBER(s, l, p, h, f) {	\
	.mode   = OPTION_NUMBER,	\
	.snam   = __opt_snam(s),	\
	.lnam   = __opt_lnam(l),	\
	.__lnam = l,			\
	.ptr    = __opt_ptr(p, u32),	\
	.argh   = "N",			\
	.usage  = h,			\
	.flags  = f,			\
}

#define OPT_STRING(s, l, p, h) __OPT_STRING(s, l, p, h, OPT_NO_NEG)
#define __OPT_STRING(s, l, p, h, f) ___OPT_STRING(s, l, p, 0, h, f)
#define ___OPT_STRING(s, l, p, a, h, f) ____OPT_STRING(s, l, p, 0, a, h, f)
#define ____OPT_STRING(s, l, p, v, a, h, f) {	\
	.mode   = OPTION_STRING,		\
	.snam   = __opt_snam(s),		\
	.lnam   = __opt_lnam(l),		\
	.__lnam = l,				\
	.ptr    = __opt_ptr(p, const xchar *),	\
	.val    = v,				\
	.argh   = a,				\
	.usage  = h,				\
	.flags  = f,				\
}

#define OPT_FILENAME(s, l, p, h) __OPT_FILENAME(s, l, p, h, OPT_NO_NEG)
#define __OPT_FILENAME(s, l, p, h, f) ___OPT_STRING(s, l, p, N_("path"), h, f)

#define OPT_COMMAND(l, p, c) __OPT_COMMAND(l, p, c, 0)
#define __OPT_COMMAND(l, p, c, f) {			\
	.mode   = OPTION__COMMAND,			\
	.lnam   = __opt_lnam(l),			\
	.__lnam = l,					\
	.ptr    = __opt_ptr(p, cmd_cb),	\
	.cmd    = c,					\
	.flags  = f,					\
}

#define OPT_CMDMODE(s, l, p, v, h) __OPT_CMDMODE(s, l, p, v, h, OPT_NO_NEG)
#define __OPT_CMDMODE(s, l, p, v, h, f) {	\
	.mode   = OPTION_CMDMODE,		\
	.snam   = __opt_snam(s),		\
	.lnam   = __opt_lnam(l),		\
	.__lnam = l,				\
	.ptr    = p,				\
	.val    = v,				\
	.usage  = h,				\
	.flags  = OPT_NO_ARG | (f),		\
}

#endif /* NG39_PARAM_H */
