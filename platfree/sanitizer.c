// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include <sanitizer/asan_interface.h>

/*
 * See also: https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
 */
const char *__asan_default_options(void)
{
	return "abort_on_error=1:"
	       "disable_coredump=0";
}
