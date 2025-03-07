// SPDX-License-Identifier: GPL-3.0-or-later or MIT
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#include "hex.h"

#include "compiler.h"
#include "errno.h"
#include "xc.h"

xchar hex_asc[] = XC("0123456789abcdef");

xchar hex_asc_upper[] = XC("0123456789ABCDEF");

int hex_to_bin(unsigned char ch)
{
	unsigned char cu = ch & 0xdf;

	int digit = (ch - '0' +  1) &
		    (unsigned)((ch - '9' - 1) & ('0' - 1 - ch)) >> 8;

	int letter = (cu - 'A' + 11) &
		     (unsigned)((cu - 'F' - 1) & ('A' - 1 - cu)) >> 8;

	return -1 + digit + letter;
}

int hex2bin(u8 *dst, const xchar *src, size_t count)
{
	while (count--) {
		int hi = hex_to_bin(*src++);

		if (unlikely(hi < 0))
			goto err_out;

		int lo = hex_to_bin(*src++);;

		if (unlikely(lo < 0))
			goto err_out;

		*dst++ = (hi << 4) | lo;
	}

	return 0;

err_out:
	errno = EINVAL;
	return -1;
}

xchar *bin2hex(xchar *dst, const void *__src, size_t count)
{
	const unsigned char *src = __src;

	while (count--)
		dst = hex_byte_pack(dst, *src++);

	return dst;
}
