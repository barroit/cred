/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include_next <stdlib.h>

#ifndef NG39_NO_STD_HDR_PATCH
#ifndef NG39_STDLIB_PATCH_H
#define NG39_STDLIB_PATCH_H

#ifdef CONFIG_ENABLE_GUI
# define EXIT_CONOUT 0x3939
#else
# define EXIT_CONOUT 0
#endif

#ifdef _WIN32

#include "attr.h"

void __exit_show_confirm(void);

void __noreturn __exit(int status);

#define wgetenv _wgetenv

int setenv(const char *name, const char *value, int overwrite);
int wsetenv(const wchar_t *name, const wchar_t *value, int overwrite);

#endif /* _WIN32 */

#endif /* NG39_STDLIB_PATCH_H */
#endif /* NG39_NO_STD_HDR_PATCH */
