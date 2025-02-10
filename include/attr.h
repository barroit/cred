/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright 2024 Jiamu Sun <barroit@linux.com>
 */

#ifndef	NG39_ATTR_H
#define NG39_ATTR_H

#define __attr __attribute__

#define __maybe_unused __attr((__unused__))

#define __noreturn __attr((__noreturn__))

#define __pure __attr((__pure__))

/*
 * Due to compatibility, the usage of __nonnull() is different than other
 * attributes.
 */
#ifndef __nonnull
# define __nonnull(...) __attr((__nonnull__ __VA_ARGS__))
#endif

#ifdef ANSI
# define __printf(m, n) __attr((__format__(__printf__, m, n))) __nonnull((m))
#else
# define __printf(m, n)
#endif

#define __cold __attribute__((__cold__))

#if __has_attribute(__access__)
# define __read_only(...) __attr((__access__(__read_only__, __VA_ARGS__)))
#else
# define __read_only(...)
#endif

#define __warn_unused_result __attr((__warn_unused_result__))

#define __leaf __attr((__leaf__))

#undef __always_inline
#define __always_inline inline __attr((__always_inline__))

#define __used __attr((__used__))

#define __constructor __attr((__constructor__))

#define __section(x) __attr((__section__(x)))

#define __cleanup(x) __attr((__cleanup__(x)))

#define __malloc __attr((__malloc__))

#endif /* NG39_ATTR_H */
