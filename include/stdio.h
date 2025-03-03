/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include_next <stdio.h>

#ifndef NG39_NO_STD_HDR_PATCH
#ifndef NG39_STDIO_PATCH_H
#define NG39_STDIO_PATCH_H

#ifdef _WIN32

#define wfreopen _wfreopen

#endif

#endif /* NG39_STDIO_PATCH_H */
#endif /* NG39_NO_STD_HDR_PATCH */
