// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

#include <stdlib.h>
#include <windows.h>

#include "legacy_cons.h"
#include "corecmd.h"
#include "proc.h"
#include "udef.h"

#ifdef CONFIG_ENABLE_WCHAR
# define WinMain wWinMain
#endif

#define argc __argc
#ifdef CONFIG_ENABLE_WCHAR
# define argv __wargv
#else
# define argv __argv
#endif

int WinMain(HINSTANCE app, HINSTANCE prev_app, xchar *cmd, int wd_cntl)
{
	MSG message;

	udef_no_console = proc_no_console('?');
	if (!udef_no_console)
		leg_cons_attach();

	cmd_main(argc, (const xchar **)argv);

	while (GetMessage(&message, NULL, 0, 0) > 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	exit(0);
}
