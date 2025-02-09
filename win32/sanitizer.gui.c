// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include <sanitizer/asan_interface.h>

#include <stdlib.h>

#include "init.h"

INIT_ROUTINE(death_cb)
{
	__sanitizer_set_death_callback(__exit_show_step);
}
