// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "pathwalk.h"

const xchar *pw_skip_root(const xchar *name)
{
	return name;
}

const xchar *pw_skip_root_back(const xchar *start, const xchar *end)
{
	return start;
}
