#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later or MIT

set -e

git switch brukit
git pull

git switch master
set +e

if git merge --no-edit --signoff brukit; then
	exit
fi

for f in $(cat .pickignore); do
	if [[ -f $f ]]; then
		git rm $f
	fi
done

if [[ -z $(git diff --name-only --diff-filter=U) ]]; then
	git merge --continue
	git commit -s --amend --no-edit
fi
