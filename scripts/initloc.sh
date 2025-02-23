#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later

ln -s ../../.hooks/pre-commit.sh .git/hooks/pre-commit
ln -s ../../.hooks/prepare-commit-msg.sh .git/hooks/prepare-commit-msg

git remote add brukit $(grep brukit .remote | cut -f2)
git remote set-url origin $(grep this .remote | cut -f2)

git branch --set-upstream-to origin/master

git fetch brukit master
git switch --track brukit/master -c brukit

git switch master
