# SPDX-License-Identifier: GPL-3.0-or-later or MIT

include(CTest)
if(UNIX)
  set(bash bash)
elseif(WIN32)
  include_scp(${TREE}/lib/cmake.bash)
  set(bash ${BASH_EXECUTABLE})
endif()

set(TEST ${TREE}/tests)
set(cmd "${bash}" ${TEST}/libtest.sh ${TREE}/scripts/libutil.sh)

file(GLOB prog_test *.sh)
list(FILTER prog_test EXCLUDE REGEX "(libtest)\\.sh")

foreach(name ${prog_test})
  get_filename_component(name ${name} NAME_WE)

  add_test(NAME cmd_${name} COMMAND ${cmd} ${TEST}/${name}.sh ../${exec_name})
endforeach()

add_subdirectory(param)
add_subdirectory(unit)

add_executable(masarg masarg.c)
