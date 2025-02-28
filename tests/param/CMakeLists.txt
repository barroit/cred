# SPDX-License-Identifier: GPL-3.0-or-later

set(PARAM ${TOP}/tests/param)

file(GLOB param_src *.c)
list(FILTER param_src EXCLUDE REGEX "(main)\\.c")

link_libraries(command OpenSSL::CryptoSO SQLite::SQLiteSO)

foreach(name ${param_src})
  get_filename_component(name ${name} NAME_WE)
  set(fake ${name}.p)

  add_executable(${name}.p ${name}.c main.c)
  add_dependencies(${name}.p genconfig feature)

  add_test(NAME prm_${name} COMMAND ${TC} ${PARAM}/${name}.sh ./${name}.p)
endforeach()

if(WIN32)
  fixup_runtime(${fake} ${OPENSSL_RUNTIME_LIB} ${SQLITE_RUNTIME_LIB})
endif()
