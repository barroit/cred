/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

/*
 * This file provides only mapping functions that redirect calls to wchar/char
 * function variants but does not define them.
 */

#ifndef NG39_XCF_H
#define NG39_XCF_H

#define __mbslen_mb    __mbslen
#define __strchrnul_mb __strchrnul
#define __strcmp_mb    strcmp
#define __strdup_mb    xstrdup
#define __strlen_mb    strlen
#define __stpncpy_mb   strncpy
#define __strncmp_mb   strncmp
#define __strrchr_mb   strrchr
#define __vsnprintf_mb vsnprintf
#define __ispunct_mb   ispunct
#define __isspace_mb   isspace
#define __isxdigit_mb  isxdigit
#define __tolower_mb   tolower
#define __mbsws_mb     mbsws

#define __mbslen_wc    wcslen
#define __strchrnul_wc __strchrnul
#define __strcmp_wc    wcscmp
#define __strdup_wc    xwcsdup
#define __strlen_wc    wcslen
#define __stpncpy_wc   wcsncpy
#define __strncmp_wc   wcsncmp
#define __strrchr_wc   wcsrchr
#define __vsnprintf_wc vswprintf
#define __ispunct_wc   iswpunct
#define __isspace_wc   iswspace
#define __isxdigit_wc  iswxdigit
#define __tolower_wc   towlower
#define __mbsws_wc     wcsws

#ifndef CONFIG_ENABLE_WCHAR
# define __xchar_type _mb
#else
# define __xchar_type _wc
#endif

#define __XCHAR_ALIAS(x)      ___XCHAR_ALIAS(x, __xchar_type)
#define ___XCHAR_ALIAS(...)   ____XCHAR_ALIAS(__VA_ARGS__)
#define ____XCHAR_ALIAS(x, t) x ## t

/*
 * Returns the exact length of a UTF-8 string in characters, independent of
 * the locale.
 */
#define xc_mbslen    __XCHAR_ALIAS(__mbslen)

#define xc_strchrnul __XCHAR_ALIAS(__strchrnul)
#define xc_strcmp    __XCHAR_ALIAS(__strcmp)
#define xc_strdup    __XCHAR_ALIAS(__strdup)
#define xc_strlen    __XCHAR_ALIAS(__strlen)
#define xc_strncpy   __XCHAR_ALIAS(__strncpy)
#define xc_strncmp   __XCHAR_ALIAS(__strncmp)
#define xc_strrchr   __XCHAR_ALIAS(__strrchr)
#define xc_vsnprintf __XCHAR_ALIAS(__vsnprintf)
#define xc_ispunct   __XCHAR_ALIAS(__ispunct)
#define xc_isspace   __XCHAR_ALIAS(__isspace)
#define xc_isxdigit  __XCHAR_ALIAS(__isxdigit)
#define xc_tolower   __XCHAR_ALIAS(__tolower)
#define xc_mbsws     __XCHAR_ALIAS(__mbsws)

#endif /* NG39_XCF_H */
