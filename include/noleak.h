/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_NOLEAK
#define NG39_NOLEAK

#include "types.h"

void __noleak(void *ptr, size_t size);

#ifdef CONFIG_IGNORE_ANNOT_LEAKS
# define noleak(var) __noleak(&(var), sizeof(var))
#else
# define noleak(var) do {} while (0)
#endif

#endif /* NG39_NOLEAK */
