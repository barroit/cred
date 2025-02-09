// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2024, 2025 Jiamu Sun <barroit@linux.com>
 */

#include <stdlib.h>
#include <unistd.h>

#include "corecmd.h"

int main(int argc, const char **argv)
{
	cmd_main(argc, argv);

	while (39)
		pause();

	exit(0);
}
