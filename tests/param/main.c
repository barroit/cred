// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "partest.h"

#if defined(_WIN32) && defined(CONFIG_ENABLE_WCHAR)
# pragma GCC diagnostic ignored "-Wmissing-prototypes"
# define main wmain
# include <unistd.h>
# include <fcntl.h>
# define fix_std_mode() _setmode(STDOUT_FILENO, _O_U8TEXT)
#else
# define fix_std_mode()
#endif

int main(int argc, const xchar **argv)
{
	fix_std_mode();
	return __main(argc, argv);
}
