// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "console.h"

#include <windows.h>
#include <stdio.h>

#include "compiler.h"
#include "gettext.h"
#include "termas.h"
#include "xc.h"

#ifdef CONFIG_WIDE_CHAR
# define freopen _wfreopen
#endif

static HWND console;

static int resize_window(short size)
{
	int ret = -1;
	DWORD errnum;
	HANDLE con = CreateFile(XC("CONOUT$"), GENERIC_READ | GENERIC_WRITE,
				0, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL);

	BUG_ON(con == INVALID_HANDLE_VALUE);

	CONSOLE_SCREEN_BUFFER_INFO info;
	int err = !GetConsoleScreenBufferInfo(con, &info);

	if (err)
		goto cleanup;

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

	err = !SetConsoleWindowInfo(con, TRUE, &rect);
	if (err)
		goto reset_window_size;

	err = !SetConsoleScreenBufferSize(con, coord);
	if (err)
		goto reset_window_size;

	ret = 0;

	if (0) {
reset_window_size:
		errnum = GetLastError();
		SetConsoleScreenBufferSize(con, info.dwSize);

		SetLastError(errnum);
	}

cleanup:
	errnum = GetLastError();

	CloseHandle(con);

	SetLastError(errnum);
	return ret;
}

void console_attach(void)
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

	err = resize_window(CONFIG_LINE_WRAP + 4);
	if (err)
		warn_winerr(_("failed to resize console window"));

	console = GetConsoleWindow();
	console_hide();

	err = !SetConsoleOutputCP(CP_UTF8);
	if (err)
		warn_winerr(_("failed to set CONOUT$ codepage to UTF-8"));
}

void console_show(void)
{
	ShowWindow(console, SW_SHOW);
}

void console_hide(void)
{
	ShowWindow(console, SW_HIDE);
}
