#!/usr/bin/bash
# SPDX-License-Identifier: GPL-3.0-or-later

sed -i.bak '/^. Please enter the commit message/,/^#$/d' $1
