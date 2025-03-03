// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2024 Jiamu Sun <barroit@linux.com>
 */

#include "termas.h"

#include <fcntl.h>
#include <unistd.h>
#include <windows.h>

#include "calc.h"

const char *strwinerr(void)
{
	static char buf[256];
	DWORD errnum = GetLastError();
	DWORD langid = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
	DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM |
		      FORMAT_MESSAGE_IGNORE_INSERTS;

	int err = !FormatMessageA(flags, NULL, errnum,
				  langid, buf, sizeof_array(buf), NULL);
	BUG_ON(err);

	errno = errnum;
	return buf;
}

void __termas_output_mode(int mode)
{
	switch (mode) {
	case TM_O_CHAR:
		mode = _O_TEXT;
		break;
	case TM_O_WCHAR:
		mode = _O_U16TEXT;
	}

	fflush(stdout);
	fflush(stderr);

	_setmode(STDOUT_FILENO, mode);
	_setmode(STDERR_FILENO, mode);
}
