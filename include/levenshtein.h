/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_LEVENSHTEIN_H
#define NG39_LEVENSHTEIN_H

#include "types.h"

struct lev_weight {
	uint add;
	uint del;
	uint sub;
	uint swp;
};

uint levenshtein(const xchar *s1,
		 const xchar *s2, const struct lev_weight *wt);

#endif /* NG39_LEVENSHTEIN_H */
