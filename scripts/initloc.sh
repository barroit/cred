#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later or MIT

if [[ ! $1 ]]; then
	>&2 echo missing repo url
fi

ln -s ../../.hooks/pre-commit.sh .git/hooks/pre-commit
ln -s ../../.hooks/prepare-commit-msg.sh .git/hooks/prepare-commit-msg

git remote add brukit $(git remote get-url origin)
git remote set-url origin $1
git branch --set-upstream-to origin/master

git switch -c brukit HEAD^
git fetch brukit
git branch --set-upstream-to brukit/master
