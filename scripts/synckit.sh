#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later or MIT

set -e

git switch brukit
git pull brukit master

git switch master
set +e

lic=$(grep license$'\t' .program | cut -f2)
lic_old=$(grep license_old .program | cut -f2)

cat <<EOF > .git/MERGE_MSG.$$
Merge branch 'brukit'

This commit brings the state in sync with brukit:
$(git show -s --format='%h ("%s")' brukit)

Signed-off-by: $(git var GIT_AUTHOR_IDENT | cut -d' ' -f-3)
EOF

trap 'rm .git/MERGE_MSG.$$' EXIT

cp .git/MERGE_MSG.$$ .git/MERGE_MSG
if git merge --no-ff --no-commit --no-edit brukit; then
	is_ff=1
else
	for f in $(cat .pickignore); do
		if [[ -f $f ]]; then
			git rm $f
		fi
	done
fi

scripts/fix-license.sh "$lic_old" "$lic"

cp .git/MERGE_MSG.$$ .git/MERGE_MSG
if [[ -z $(git diff --name-only --diff-filter=U) || $is_ff ]]; then
	git add .
	git merge --continue
fi
