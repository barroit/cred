// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "proc.h"

#include <stdlib.h>

#include "compiler.h"

#define PROC_NO_CONSOLE PROGRAM_NAME "_NO_CONSOLE"

int proc_no_console(char act)
{
	char __str[2] = { 0 };
	char *str = __str;

	str[0] = act;
	switch (str[0]) {
	case 'y':
	case 'n':
		setenv(PROC_NO_CONSOLE, str, 1);
		return 0;
	case '?':
		str = getenv(PROC_NO_CONSOLE);
		return str && str[0] == 'y' && !str[1];
	}

	trap();
}
