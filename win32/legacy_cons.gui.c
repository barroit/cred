// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "legacy_cons.h"

#include <windows.h>
#include <stdio.h>

#include "compiler.h"
#include "gettext.h"
#include "termas.h"
#include "xc.h"

#ifdef CONFIG_ENABLE_WCHAR
# define freopen _wfreopen
#endif

static HWND conwin;
static HANDLE conout;

static int resize_window(short size)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	int err = !GetConsoleScreenBufferInfo(conout, &info);

	if (err)
		return -1;

	DWORD errnum;
	COORD coord = {
		.X = size,
		.Y = info.dwSize.Y,
	};
	SMALL_RECT rect = {
		.Left   = 0,
		.Top    = 0,
		.Right  = (short)(coord.X - 1),
		.Bottom = 20,
	};

	err = !SetConsoleWindowInfo(conout, TRUE, &rect);
	if (err)
		goto err_out;

	err = !SetConsoleScreenBufferSize(conout, coord);
	if (err)
		goto err_out;

	return 0;

err_out:
	errnum = GetLastError();
	SetConsoleScreenBufferSize(conout, info.dwSize);

	SetLastError(errnum);
	return -1;
}

void leg_cons_attach(void)
{
	int err;
	FILE *stream;
	
	err = !AllocConsole();
	BUG_ON(err);

	stream = freopen(XC("CONIN$"), XC("r"), stdin);
	BUG_ON(!stream);

	stream = freopen(XC("CONOUT$"), XC("w"), stdout);
	BUG_ON(!stream);

	stream = freopen(XC("CONOUT$"), XC("w"), stderr);
	BUG_ON(!stream);

	conout = CreateFile(XC("CONOUT$"), GENERIC_READ | GENERIC_WRITE, 0,
			    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	BUG_ON(conout == INVALID_HANDLE_VALUE);

	err = resize_window(CONFIG_LINE_WRAP + 4);
	if (err)
		warn_winerr(_("failed to resize console window"));

	conwin = GetConsoleWindow();
	BUG_ON(!conwin);

	leg_cons_hide();

	err = !SetConsoleOutputCP(CP_UTF8);
	BUG_ON(err);

	DWORD mode;

	err = !GetConsoleMode(conout, &mode);
	BUG_ON(err);

	mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	err = !SetConsoleMode(conout, mode);
	BUG_ON(err);
}

void leg_cons_show(void)
{
	ShowWindow(conwin, SW_SHOW);
}

void leg_cons_hide(void)
{
	ShowWindow(conwin, SW_HIDE);
}
