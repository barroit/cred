// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

#include "param.h"

#include "bitopt.h"
#include "iter.h"
#include "list.h"
#include "strbuf.h"
#include "strconv.h"
#include "string.h"
#include "strlist.h"
#include "strtox.h"
#include "termas.h"
#include "xalloc.h"
#include "xchar.h"

/*
 * Windows SDK sucks!
 */
#ifdef __argc
# undef __argc
#endif

#define opt_for_each(pos, opts) \
	for (pos = opts; (pos)->mode != OPTION__END; (pos)++)

#define OPT_SHORT_OPT (1 << 0)
#define OPT_LONG_OPT  (1 << 1)
#define OPT_UNSET (1 << 2)

#define __setopt(name) opt_set_ ## name

#define NO_NO_ARG (-1U << 31)

struct param {
	int argc;
	const xchar **argv;

	int outc;
	const xchar **outv;

	struct opt *opts;
	const char **usage;

	u32 flags;

	struct list_head mode;
};

struct abbrev {
	struct opt *opt;
	u32 flags;
};

struct cmdmode {
	struct opt *opt;
	u32 flags;

	struct list_head list;
};

static void __noreturn show_help(const char **usage,
				 struct opt *opts, int is_err);

static struct strbuf __cmdpath = SB_INIT;
static char *cmdpath;

static int has_command(struct opt *opts)
{
	struct opt *opt;

	opt_for_each(opt, opts) {
		if (opt->mode == OPTION__COMMAND)
			return 1;
	}

	return 0;
}

static void cmdpath_append(const xchar *name)
{
	if (__cmdpath.len)
		sb_putc(&__cmdpath, XC(' '));
	else
		name = XC(PROGRAM_NAME);

	sb_puts(&__cmdpath, name);

	if (IS_ENABLED(CONFIG_WIDE_CHAR)) {
		if (cmdpath)
			free(cmdpath);
		cmdpath = sb_mb_str_fb(&__cmdpath, "����");
	} else {
		cmdpath = (char *)__cmdpath.buf;
	}
}

static void cmdmode_destroy(struct list_head *mode)
{
	struct cmdmode *cm, *tmp;

	list_for_each_entry_safe(cm, tmp, mode, list) {
		list_del(&cm->list);
		free(cm);
	}
}

static char *pretty_opt_name(struct opt *opt, u32 flags)
{
	struct strbuf sb = SB_INIT;

	if (flags & OPT_SHORT_OPT)
		sb_printf(&sb, XC("-%c"), opt->snam);
	else
		sb_printf(&sb, XC("--%s%s"),
			  flags & OPT_UNSET ? XC("no-") : XC(""), opt->lnam);
	
	char *ret = sb_mb_str_fb(&sb, "���");

	sb_destroy(&sb);
	return ret;
}

static char *pretty_arg_name(const xchar *arg, const char *fb)
{
	char *ret = (char *)arg;

	if (IS_ENABLED(CONFIG_WIDE_CHAR))
		mb_wcstombs_fb(&ret, (wchar_t *)arg, fb);

	return ret;
}

static void cmdmode_record(struct list_head *mode,
			   struct opt *opt, u32 flags)
{
	struct cmdmode *cm;

	list_for_each_entry(cm, mode, list) {
		if (cm->opt == opt)
			continue;

		char *n1 = pretty_opt_name(cm->opt, cm->flags);
		char *n2 = pretty_opt_name(opt, flags);

		die(_("options %s and %s cannot be used together"), n1, n2);
	}

	cm = xcalloc(1, sizeof(*cm));
	list_add(&cm->list, mode);

	cm->opt = opt;
	cm->flags = flags;
}

static int parse_command(struct opt *opts, const xchar *cmd)
{
	struct opt *opt;

	opt_for_each(opt, opts) {
		if (opt->mode != OPTION__COMMAND)
			continue;

		if (xc_strcmp(opt->lnam, cmd) != 0)
			continue;

		*(cmd_cb *)opt->ptr = opt->cmd;
		return 39;
	}

	char *name = pretty_arg_name(cmd, "����");

	die(_("unknown command `%s', see '%s -h'"), name, cmdpath);
}

static void __setopt(bit)(struct opt *opt, const xchar *arg, u32 flags)
{
	u32 *p = opt->ptr;

	if (flags & OPT_UNSET)
		*p &= ~opt->val;
	else
		*p |= opt->val;
}

static void __setopt(switch)(struct opt *opt, const xchar *arg, u32 flags)
{
	int *p = opt->ptr;

	*p = flags & OPT_UNSET ? 0 : 1;
}

static void __setopt(number)(struct opt *opt, const xchar *arg, u32 flags)
{
	if (flags & OPT_UNSET) {
		*(u32 *)opt->ptr = 0;
		return;
	}

	int err;

	err = stx_strtou32(arg, 0, opt->ptr);
	if (!err)
		return;

	char *name = pretty_opt_name(opt, flags);
	char *val = pretty_arg_name(arg, "���");

	die(_("option %s has an invalid integer value `%s'"), name, val);
}

static void __setopt(string)(struct opt *opt, const xchar *arg, u32 flags)
{
	const xchar **p = opt->ptr;

	if (flags & OPT_UNSET)
		*p = NULL;
	else if (arg)
		*p = arg;
	else if (opt->flags & OPT_OPT_ARG)
		*p = (const xchar *)opt->val;
}

static void __setopt(cmdmode)(struct opt *opt, const xchar *arg, u32 flags)
{
	int *p = opt->ptr;

	*p = opt->val;
}

static void set_opt_val(struct param *ctx,
			struct opt *opt, const xchar *arg, u32 flags)
{
	static typeof(__setopt(switch)) *map[__OPTION__SIZE] = {
		[OPTION_BIT]     = __setopt(bit),
		[OPTION_SWITCH]  = __setopt(switch),
		[OPTION_NUMBER]  = __setopt(number),
		[OPTION_STRING]  = __setopt(string),
		[OPTION_CMDMODE] = __setopt(cmdmode),
	};

	map[opt->mode](opt, arg, flags);

	if (opt->mode == OPTION_CMDMODE)
		cmdmode_record(&ctx->mode, opt, flags);
}

static const xchar *parse_shrt_opt(struct param *ctx, const xchar *args)
{
	struct opt *opt;
	const xchar *arg = NULL;

	xchar snam = args[0];
	const xchar *ret = &args[1];

	opt_for_each(opt, ctx->opts) {
		if (opt->snam != snam)
			continue;

		if (opt->flags & OPT_NO_ARG) {
			arg = NULL;
		} else if (args[1]) {
			arg = &args[1];
			ret = XC("");
		} else if (ctx->argc > 1) {
			arg = ctx->argv[0];
			ctx->argc--;
			ctx->argv++;
		} else if (!(opt->flags & OPT_OPT_ARG)) {
			die(_("option -%c requires a value"), opt->snam);
		}

		set_opt_val(ctx, opt, arg, OPT_SHORT_OPT);
		return ret;
	}

	die(_("unknown option -%c"), (char)snam);
}

static void opt_abbrev(struct opt *opt, u32 flags,
		       struct abbrev *abbrev, struct abbrev *ambig)
{
	if (abbrev->opt && abbrev->flags != flags) {
		ambig->opt = abbrev->opt;
		ambig->flags = abbrev->flags;
	}

	abbrev->opt = opt;
	abbrev->flags = flags;
}

static int __parse_long_opt(struct param *ctx, struct opt *opt,
			    const xchar *arg, u32 argf, u32 optf)
{
	int unset = argf & OPT_UNSET;

	if (arg[0] == '=') {
		if (opt->flags & OPT_NO_ARG || unset)
			die(_("option `%s%s' takes no value"),
			    unset ? "no-" : "", opt->__lnam);
		arg += 1;
	} else if (arg[0]) {
		return -1;
	} else if ((opt->flags & OPT_NO_ARG) || unset) {
		arg = NULL;
	} else if (ctx->argc > 1) {
		arg = ctx->argv[1];
		ctx->argc--;
		ctx->argv++;
	} else if (!(opt->flags & OPT_OPT_ARG)) {
		die(_("option `%s%s' requires a value"),
		    unset ? "no-" : "", opt->__lnam);
	}

	set_opt_val(ctx, opt, arg, argf ^ optf);
	return 0;
}

static void parse_long_opt(struct param *ctx, const xchar *arg)
{
	int err;
	struct opt *opt;

	const xchar *anam = arg;
	const xchar *asep = xc_strchrnul(arg, '=');
	u32 aflg = OPT_LONG_OPT;

	struct abbrev abbrev = { 0 };
	struct abbrev ambig = { 0 };

	err = strskip2(anam, XC("no-"), &anam);
	if (!err) {
		err = strskip2(anam, XC("no-"), &anam);

		if (err)
			aflg |= OPT_UNSET;
		else
			aflg |= NO_NO_ARG;
	}

	opt_for_each(opt, ctx->opts) {
		if (!opt->lnam || opt->mode == OPTION__COMMAND)
			continue;

		const xchar *onam = opt->lnam;
		u32 oflg = OPT_LONG_OPT;
		int no_neg = opt->flags & OPT_NO_NEG;
		const xchar *rest = NULL;

		err = strskip2(onam, XC("no-"), &onam);
		if (!err)
			oflg |= OPT_UNSET;
		else if (aflg & NO_NO_ARG)
			continue;

		if ((aflg ^ oflg) & OPT_UNSET && no_neg)
			continue;

		err = strskip2(anam, onam, &rest);
		if (!err) {
			err = __parse_long_opt(ctx, opt, rest, aflg, oflg);
			if (err)
				continue;
			return;
		}

		if (xc_strncmp(onam, anam, asep - anam) == 0)
			opt_abbrev(opt, aflg ^ oflg, &abbrev, &ambig);

		if (!no_neg && strskip(XC("no-"), arg) != NULL)
			opt_abbrev(opt, OPT_UNSET ^ oflg, &abbrev, &ambig);
	}

	if (ambig.opt) {
		char *nam = pretty_arg_name(arg, "���");
		char *nam1 = pretty_opt_name(ambig.opt, ambig.flags);
		char *nam2 = pretty_opt_name(abbrev.opt, abbrev.flags);

		die(_("ambiguous option --%s, could be %s or %s"),
		    nam, &nam1[2], &nam2[2]);
	} else if (abbrev.opt) {
		arg = XC("");
		if (*asep)
			arg = asep;

		err = __parse_long_opt(ctx, abbrev.opt,
				       arg, aflg, abbrev.flags);
		BUG_ON(err);
	} else {
		char *nam = pretty_arg_name(arg, "���");

		die(_("unknown option --%s"), nam);
	}
}

static int parse_cmd_arg(struct param *ctx)
{
	const xchar *str = ctx->argv[0];

	if (str[0] != '-') {
		if (ctx->flags & PRM_RET_ARG)
			return 39;
		else if (ctx->flags & PRM_PAR_CMD)
			return parse_command(ctx->opts, str);
		else if (ctx->flags & PRM_NO_ARG)
			die(_("'%s' takes no arguments, but got `%s'"),
			    cmdpath, str);

		ctx->outv[ctx->outc] = str;
		ctx->outc += 1;
	} else if (str[1] != '-') {
		str += 1;

		if (!(ctx->flags & PRM_NO_HELP) &&
		    str[0] == 'h' && str[1] == 0)
			show_help(ctx->usage, ctx->opts, 0);
		else if (str[0] == 0)
			die(_("unknown option -"));

		while (*str)
			str = parse_shrt_opt(ctx, str);
	} else {
		str += 2;

		/* end of options ("--") */
		if (str[0] == 0) {
			ctx->argc--;
			ctx->argv++;
			return 39;
		}

		if (!(ctx->flags & PRM_NO_HELP) &&
		    xc_strcmp(str, XC("help")) == 0)
			show_help(ctx->usage, ctx->opts, 0);

		parse_long_opt(ctx, str);
	}

	return 0;
}

int parse_param(int argc, const xchar **argv,
		const char **usage, struct opt *opts, u32 flags)
{
	int __argc = argc - 1;

	if (has_command(opts)) {
		BUG_ON(flags & PRM_RET_ARG);
		flags |= PRM_PAR_CMD;
	}

	struct param ctx = {
		.argc  = __argc,
		.argv  = argv + 1,

		.outc  = 0,
		.outv  = argv,

		.opts  = opts,
		.usage = usage,

		.flags = flags,

		.mode  = LIST_HEAD_INIT(ctx.mode),
	};

	cmdpath_append(argv[0]);

	while (ctx.argc) {
		int ret = parse_cmd_arg(&ctx);

		if (unlikely(ret == 39))
			break;

		ctx.argc--;
		ctx.argv++;
	}

	if (!list_is_empty(&ctx.mode))
		cmdmode_destroy(&ctx.mode);

	if (ctx.argc)
		memmove(&ctx.outv[ctx.outc],
			ctx.argv, ctx.argc * sizeof(*ctx.argv));

	int ret = ctx.outc + ctx.argc;

	if (flags & PRM_PAR_CMD && ret == 0) {
		int err = 1;

		if (!(flags & PRM_OPT_CMD) && __argc)
			error(_("'%s' requires a subcommand\n"), cmdpath);
		else
			err = 0;

		show_help(ctx.usage, ctx.opts, err);
	}

	argc = ctx.outc + ctx.argc;
	ctx.outv[argc] = 0;
	return argc;
}

static void show_cmd_usage(FILE *stream, const char **usage)
{
	const char *pref = "usage: ";
	size_t __len = strlen("usage: ");

	STRLIST(sl, SL__STORE_CHR);

	while (*usage) {
		const char *line = *usage;
		const char *rest = line;

		while (isalpha(*rest) || isspace(*rest))
			rest++;

		size_t len = rest - line;
		size_t pad = __len + len;
		size_t wrap = CONFIG_LINE_WRAP - __len;
		char *str;

		if ((CONFIG_LINE_WRAP >> 1) > pad)
			wrap -= len;

		fprintf(stream, "%*s%.*s", (int)__len, pref, (int)len, line);

		if (!*rest) {
			putc('\n', stream);
			goto next;
		}
		sl_read_line_chr(&sl, rest, wrap);

		str = sl_pop_chr(&sl);
		fputs(str, stream);
		putc('\n', stream);
		free(str);

		while ((str = sl_pop_chr(&sl))) {
			fprintf(stream, "%*s%s\n", (int)pad, "", str);
			free(str);
		}

next:
		usage++;
		pref = "or: ";
	}

	sl_destroy(&sl);
	putc('\n', stream);
}

static void show_opt_usage(FILE *stream, struct opt *opts)
{
	int cnt = 0;
	struct opt *opt;

	STRLIST(sl, SL__STORE_CHR);

	opt_for_each(opt, opts) {
		switch (opt->mode) {
		case OPTION__GROUP:
			putc('\n', stream);
			fputs(opt->__lnam, stream);
			putc('\n', stream);
		case OPTION__COMMAND:
			continue;
		}

		const char *fmt;
		size_t len = CONFIG_HELP_OPT_INDENT;

		fprintf(stream, "%*s", (int)len, "");

		if (opt->snam)
			len += fprintf(stream, "-%c, ", opt->snam);

		int no_neg = opt->flags & OPT_NO_NEG;

		fmt = no_neg ? "--%s" : "--[no-]%s";
		len += fprintf(stream, fmt, opt->__lnam);

		if (opt->argh) {
			char *occur = strpbrk(opt->argh, "()<>[]|");
			int no_brac = occur != NULL;

			if (opt->flags & OPT_OPT_ARG)
				fmt = no_brac ? "[=%s]" : "[=<%s>]";
			else
				fmt = no_brac ? " %s" : " <%s>";
			len += fprintf(stream, fmt, opt->argh);
		}

		if (len >= CONFIG_HELP_OPT_WRAP) {
			putc('\n', stream);
			len = 0;
		}

		size_t __pad = CONFIG_HELP_OPT_WRAP + 2;
		size_t pad = __pad - len;
		size_t wrap = CONFIG_LINE_WRAP - __pad;
		char *str;

		sl_read_line_chr(&sl, _(opt->usage), wrap);

		str = sl_pop_chr(&sl);
		fprintf(stream, "%*s%s\n", (int)pad, "", str);
		free(str);

		while ((str = sl_pop_chr(&sl))) {
			fprintf(stream, "%*s%s\n", (int)__pad, "", str);
			free(str);
		}

		cnt++;
	}

	sl_destroy(&sl);
	if (cnt)
		putc('\n', stream);
}

static void show_cmd_ext_usage(FILE *stream, const char **usage)
{
	while (*usage++);

	uint lines = 0;

	while (*usage) {
		if (lines++)
			putc('\n', stream);

		fputs(_(*usage), stream);
		putc('\n', stream);

		usage++;
	}
}

static void show_help(const char **usage, struct opt *opts, int is_err)
{
	FILE *stream = is_err ? stderr : stdout;

	show_cmd_usage(stream, usage);

	show_opt_usage(stream, opts);

	show_cmd_ext_usage(stream, usage);

	exit(EXIT_CONOUT);
}
