// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "param.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strlist.h"

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

void param_show_help(const char **usage, struct opt *opts, int is_err)
{
	FILE *stream = is_err ? stderr : stdout;

	show_cmd_usage(stream, usage);

	show_opt_usage(stream, opts);

	show_cmd_ext_usage(stream, usage);

	exit(EXIT_CONOUT);
}
