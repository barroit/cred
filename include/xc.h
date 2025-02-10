/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_XC_H
#define NG39_XC_H

#define XC(x) __XC(x)

#ifdef CONFIG_WIDE_CHAR
# define __XC(x) L##x
#else
# define __XC(x) x
#endif

#endif /* NG39_XC_H */

