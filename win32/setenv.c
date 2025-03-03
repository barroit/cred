// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include <stdlib.h>

#include <errno.h>
#include <string.h>

int setenv(const char *name, const char *value, int overwrite)
{
	if (!name || *name == 0 || strchr(name, '=')) {
		errno = EINVAL;
		return -1;
	}

	char *prev = getenv(name);

	if (prev && !overwrite)
		return 0;

	size_t nlen = strlen(name);
	size_t vlen = strlen(value);
	size_t slen = nlen + 1 + vlen;

	char *str = malloc(slen + 1);

	if (!str) {
		errno = ENOMEM;
		return -1;
	}

	memcpy(str, name, nlen);
	str[nlen] = '=';
	memcpy(&str[nlen + 1], value, vlen);
	str[slen] = 0;

	int err = putenv(str);

	if (err) {
		free(str);
		return -1;
	}

	return 0;
}

int wsetenv(const wchar_t *name, const wchar_t *value, int overwrite)
{
	if (!name || *name == 0 || wcschr(name, L'=')) {
		errno = EINVAL;
		return -1;
	}

	if (!overwrite && wgetenv(name))
		return 0;

	size_t nl = wcslen(name);
	size_t vl = wcslen(value);
	size_t len = nl + 1 + vl;
	size_t size = (len + 1) * sizeof(wchar_t);

	wchar_t *str = malloc(size);

	if (!str) {
		errno = ENOMEM;
		return -1;
	}

	memcpy(str, name, nl * sizeof(wchar_t));
	str[nl] = L'=';
	memcpy(&str[nl + 1], value, vl * sizeof(wchar_t));
	str[len] = 0;

	int err = _wputenv(str);

	if (err) {
		free(str);
		return -1;
	}

	return 0;
}
