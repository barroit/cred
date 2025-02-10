// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

#include <stdlib.h>
#include <windows.h>

#include "corecmd.h"

#ifdef CONFIG_WIDE_CHAR
# pragma GCC diagnostic ignored "-Wmissing-prototypes" 
# define main wmain
#endif

int main(int argc, const xchar **argv)
{
	cmd_main(argc, argv);

	HANDLE thread = GetCurrentThread();

	WaitForSingleObject(thread, INFINITE);
	exit(0);
}
