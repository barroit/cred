// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "partest.h"

#if defined(_WIN32) && defined(CONFIG_WIDE_CHAR)
# pragma GCC diagnostic ignored "-Wmissing-prototypes"
# define main wmain
#endif

int main(int argc, const xchar **argv)
{
	return __main(argc, argv);
}
