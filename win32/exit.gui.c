/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "legacy_cons.h"
#include "gettext.h"
#include "udef.h"

#ifdef exit
# undef exit
#endif

void __exit_show_step(void)
{
	puts(_("\nPress any key to continue..."));
	_getch();
}

void __exit(int status)
{
	if (!udef_no_console && status) {
		leg_cons_show();
		__exit_show_step();
	}

	if (status == EXIT_CONOUT)
		status = 0;

	exit(status);
}
