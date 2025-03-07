/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * Copyright 2025 Jiamu Sun <barroit@linux.com>
 */

#ifndef NG39_HEX_H
#define NG39_HEX_H

#include "types.h"

extern xchar hex_asc[];

#define hex_asc_lo(x) hex_asc[((x) & 0x0f)]
#define hex_asc_hi(x) hex_asc[((x) & 0xf0) >> 4]

static inline xchar *hex_byte_pack(xchar *buf, u8 byte)
{
	*buf++ = hex_asc_hi(byte);
	*buf++ = hex_asc_lo(byte);

	return buf;
}

extern xchar hex_asc_upper[];

#define hex_asc_upper_lo(x) hex_asc_upper[((x) & 0x0f)]
#define hex_asc_upper_hi(x) hex_asc_upper[((x) & 0xf0) >> 4]

static inline xchar *hex_byte_pack_upper(xchar *buf, u8 byte)
{
	*buf++ = hex_asc_upper_hi(byte);
	*buf++ = hex_asc_upper_lo(byte);

	return buf;
}

int hex_to_bin(unsigned char ch);

int hex2bin(u8 *dst, const xchar *src, size_t count);

xchar *bin2hex(xchar *dst, const void *src, size_t count);

#endif /* NG39_HEX_H */
