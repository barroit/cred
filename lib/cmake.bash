# SPDX-License-Identifier: GPL-3.0-or-later
# Only makes sense on Windows.

find_package(Git REQUIRED)

get_filename_component(dir ${GIT_EXECUTABLE} DIRECTORY)

find_program(bash bash PATHS ${dir}/../bin REQUIRED)

set(BASH_EXECUTABLE ${bash} PARENT_SCOPE)
