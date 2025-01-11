/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_UNICODE_CLAUSE_H
#define NG39_UNICODE_CLAUSE_H

#include <stddef.h>

#include "attr.h"

/*
 * Check if a symbol is an end-of-clause character, such as a comma,
 * period, question mark, etc.
 */
int iseoc(wchar_t c) __leaf __pure;

#endif /* NG39_UNICODE_CLAUSE_H */
