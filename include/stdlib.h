/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_STDLIB_H
#define NG39_STDLIB_H
#include_next <stdlib.h>
#ifndef NG39_NO_STD_HDR_PATCH

#include "attr.h"

void __exit_show_step(void);

void __noreturn __exit(int status);

#ifdef CONFIG_WIN32_GUI
# define exit __exit
# define EXIT_CONOUT 0x3939
#else
# define EXIT_CONOUT 0
#endif

#ifdef _WIN32
int setenv(const char *name, const char *value, int overwrite);
#endif

#endif /* NG39_NO_STD_HDR_PATCH */
#endif /* NG39_STDLIB_H */
