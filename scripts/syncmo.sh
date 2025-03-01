#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later or MIT

set -e

echo 'preparing...'
make configure EXTOPT='-DCMAKE_C_FLAGS="-C -DINTL_PREP_MO"' >/dev/null

cd build.unix

i=$(make help | grep '\.i$' | cut -d' ' -f2)

xargs -n1 -P$(nproc) make <<< $i >/dev/null

cd ..

domain=$(grep name .program | cut -f2)

cd locale

src=$(find ../build.unix/CMakeFiles -type f -name '*.i')
lang=(zh_CN ja_JP)

xgettext --add-comments=TRANSLATORS --omit-header --no-location \
	 --from-code=UTF-8 -LC -i -k_ -kN_ -k__H_ -k__HN_ $src

if [[ $1 && $(grep $1 <<< ${lang[@]}) ]]; then
	lang=($1)
fi

for l in ${lang[@]}; do
	msgmerge -i --no-location -U $l.po messages.po
	mkdir -p $l/LC_MESSAGES
	msgfmt -o $l/LC_MESSAGES/$domain.mo $l.po
done

cd ..

echo 'cleaning...'
make configure EXTOPT='-DCMAKE_C_FLAGS=""' >/dev/null
