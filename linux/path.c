// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2024 Jiamu Sun <barroit@linux.com>
 */

#include "path.h"

#include "compiler.h"

const char *pth_executable(void)
{
	static char *name;

	if (unlikely(!name)) {
		name = delink("/proc/self/exe");
		BUG_ON(!name);
	}

	return name;
}
