/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "console.h"
#include "gettext.h"
#include "udef.h"

#ifdef exit
# undef exit
#endif

void __exit(int status)
{
	if (IS_ENABLED(CONFIG_WIN32_GUI)) {
		if (!udef_no_console && status) {
			console_show();

			puts(_("Press any key to continue..."));
			_getch();
		}
	}

	if (status == EXIT_CONOUT)
		status = 0;

	exit(status);
}
