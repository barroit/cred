/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_PARTEST_H
#define NG39_PARTEST_H

#include <stdio.h>
#include <wchar.h>

#include "types.h"

int __main(int argc, const xchar **argv);

#ifndef CONFIG_ENABLE_WCHAR
# define pprint_hex(var)     printf("39\t%s\t%02x\n", #var, var)
# define pprint_int(var)     printf("39\t%s\t%d\n", #var, var)
# define pprint_uint(var)    printf("39\t%s\t%u\n", #var, var)
# define pprint_str(var)     printf("39\t%s\t%s\n", #var, var)
# define pprint_cmd(var)     printf("39\tcmd\t%s\n", #var)
# define pprint_cmdmode(var) printf("39\tmode\t%s\n", #var)
#else
# define pprint_hex(var)     wprintf(L"39\t%ls\t%02x\n", L#var, var)
# define pprint_int(var)     wprintf(L"39\t%ls\t%d\n", L#var, var)
# define pprint_uint(var)    wprintf(L"39\t%ls\t%u\n", L#var, var)
# define pprint_str(var)     wprintf(L"39\t%ls\t%ls\n", L#var, var)
# define pprint_cmd(var)     wprintf(L"39\tcmd\t%ls\n", L#var)
# define pprint_cmdmode(var) wprintf(L"39\tmode\t%ls\n", L#var)
#endif


#endif /* NG39_PARTEST_H */
