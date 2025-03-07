// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright 2024 Jiamu Sun <barroit@linux.com>
 */

#include "path.h"

#include "compiler.h"
#include "strbuf.h"
#include "udef.h"
#include "xalloc.h"
#include "xc.h"
#include "xcf.h"

const xchar *pth_prefix(void)
{
	static xchar *name;

	if (unlikely(!name)) {
		const xchar *child = pth_executable();
		struct strbuf sb = SB_INIT;

		sb_puts(&sb, child);
		sb_pth_to_dirname(&sb);
		name = sb_detach(&sb);
	}

	return name;
}

const xchar *pth_locale(void)
{
	static xchar *name;

	if (unlikely(!name)) {
		const xchar *prefix = pth_prefix();
		struct strbuf sb = SB_INIT;

		sb_puts(&sb, prefix);
		sb_pth_append(&sb, XC("locale"));
		name = sb_detach(&sb);
	}

	return name;
}

static const xchar *pth_def_cred_home(void)
{
	static xchar *name;

	if (!name) {
		struct strbuf sb;
		const xchar *dir = pth_home();

		sb_init_ws(&sb, dir);
		sb_pth_append(&sb, XC(".cred"));

		name = sb.buf;
	}

	return name;
}

static xchar *pth_udef_cred_file(const xchar *name)
{
	if (pth_is_abs(name))
		return xc_strdup(name);

	struct strbuf sb;
	const xchar *cred_dir = udef_prefix;

	if (!UDEF_DEFINED(cred_dir) || !cred_dir[0])
		cred_dir = pth_cwd();

	sb_init_ws(&sb, cred_dir);
	sb_pth_append(&sb, name);

	return sb.buf;
}

static xchar *pth_def_cred_file(const xchar *name)
{
	struct strbuf sb;
	const xchar *cred_dir = udef_prefix;

	if (!UDEF_DEFINED(cred_dir) || !cred_dir[0])
		cred_dir = xc_getenv(XC("CRED_DIR"));
	if (!cred_dir || !cred_dir[0])
		cred_dir = pth_def_cred_home();

	sb_init_ws(&sb, cred_dir);
	sb_pth_append(&sb, name);
	return sb.buf;
}

const xchar *pth_cred_raw(void)
{
	static xchar *name;

	if (!name) {
		if (UDEF_DEFINED(udef_cred))
			name = pth_udef_cred_file(udef_cred);
		else
			name = pth_def_cred_file(XC("negi"));
	}

	return name;
}

const xchar *pth_cred_enc(void)
{
	static xchar *name;

	if (!name) {
		if (UDEF_DEFINED(udef_cred))
			name = pth_udef_cred_file(udef_cred);
		else
			name = pth_def_cred_file(XC("negi39"));
	}

	return name;
}

const xchar *pth_cred_key(void)
{
	static xchar *name;

	if (!name) {
		if (UDEF_DEFINED(udef_cred_key))
			name = pth_udef_cred_file(udef_cred_key);
		else
			name = pth_def_cred_file(XC("negi01"));
	}

	return name;
}
