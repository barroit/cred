// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "noleak.h"

#include "list.h"
#include "xalloc.h"

struct ref {
	struct list_head list;
	char carrier[];
};

static LIST_HEAD(refs);

void __noleak(void *ptr, size_t size)
{
	size_t n = sizeof(struct ref) + size;
	struct ref *ref = xmalloc(n);

	list_add(&ref->list, &refs);
	memcpy(ref->carrier, ptr, size);
}
