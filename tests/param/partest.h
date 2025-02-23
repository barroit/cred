/* SPDX-License-Identifier: GPL-3.0-or-later or MIT */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_PARTEST_H
#define NG39_PARTEST_H

#include <stdio.h>
#include <wchar.h>

#include "types.h"

int __main(int argc, const xchar **argv);

#define pprint_hex(var)  printf("39\t%s\t%02x\n", #var, var)
#define pprint_int(var)  printf("39\t%s\t%d\n", #var, var)
#define pprint_uint(var) printf("39\t%s\t%u\n", #var, var)

#ifndef CONFIG_ENABLE_WCHAR
# define pprint_str(var) printf("39\t%s\t%s\n", #var, var)
#else
# define pprint_str(var) wprintf(L"39\t%ls\t%ls\n", L#var, var)
#endif

#define pprint_cmd(var)  printf("39\tcmd\t%s\n", #var)

#define pprint_cmdmode(var) printf("39\tmode\t%s\n", #var)

#endif /* NG39_PARTEST_H */
