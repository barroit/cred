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

int winerrno(void)
{
	DWORD code = GetLastError();

	switch (code) {
	case ERROR_SUCCESS:
		return 0;
	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
		return ENOENT;
	case ERROR_ACCESS_DENIED:
	case ERROR_SHARING_VIOLATION:
		return EACCES;
	case ERROR_INVALID_HANDLE:
		return EBADF;
	case ERROR_NOT_ENOUGH_MEMORY:
	case ERROR_OUTOFMEMORY:
		return ENOMEM;
	case ERROR_INVALID_DRIVE:
		return ENODEV;
	case ERROR_WRITE_PROTECT:
		return EROFS;
	case ERROR_FILE_EXISTS:
	case ERROR_ALREADY_EXISTS:
		return EEXIST;
	case ERROR_INVALID_PARAMETER:
		return EINVAL;
	case ERROR_BROKEN_PIPE:
		return EPIPE;
	case ERROR_DISK_FULL:
		return ENOSPC;
	case ERROR_CALL_NOT_IMPLEMENTED:
		return ENOSYS;
	case ERROR_NO_MORE_FILES:
		return ENFILE;
	default:
		return -1;
	}
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
