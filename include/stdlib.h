/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_STDLIB_H
#define NG39_STDLIB_H

#include_next <stdlib.h>

#include "attr.h"

#define EXIT_CONOUT -39

#define exit __exit
void __noreturn __exit(int status);

#endif /* NG39_STDLIB_H */
