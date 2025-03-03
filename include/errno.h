/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_ERRNO_H
#define NG39_ERRNO_H

#include_next <errno.h>

#ifdef _WIN32
int winerrno(void);
#endif

#endif /* NG39_ERRNO_H */
