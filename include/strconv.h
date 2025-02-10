/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright 2024 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_CHARCONV_H
#define NG39_CHARCONV_H

#include <stddef.h>

size_t mb_wcstombs(char **__dest, const wchar_t *__src);

/*
 * If the length returned by this function is maxof(size_t), then the dest
 * buffer must not be passed to free(3).
 */
size_t mb_wcstombs_fb(char **dest, const wchar_t *src, const char *fb);

size_t wc_mbstowcs(wchar_t **__dest, const char *__src);

size_t wc_mbstowcs_fb(wchar_t **dest, const char *src, const wchar_t *fb);

#endif /* NG39_CHARCONV_H */
