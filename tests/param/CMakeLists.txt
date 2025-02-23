# SPDX-License-Identifier: GPL-3.0-or-later or MIT

set(PARAM ${TREE}/tests/param)

file(GLOB param_src *.c)
list(FILTER param_src EXCLUDE REGEX "(main)\\.c")

link_libraries(command)

foreach(name ${param_src})
  get_filename_component(name ${name} NAME_WE)

  add_executable(${name}.p ${name}.c main.c)
  add_dependencies(${name}.p genconfig feature)

  add_test(NAME prm_${name} COMMAND ${cmd} ${PARAM}/${name}.sh ./${name}.p)
endforeach()
