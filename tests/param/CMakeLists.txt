# SPDX-License-Identifier: GPL-3.0-or-later

set(prmsrc ${srctree}/tests/param)
set(prmobj ${objtree}/tests/param)

file(GLOB prms *.c)
list(FILTER prms EXCLUDE REGEX "(main)\\.c")

link_libraries(command OpenSSL::CryptoSO SQLite::SQLiteSO)

foreach(name ${prms})
  get_filename_component(name ${name} NAME_WE)
  set(fake ${name}.p)

  add_executable(${name}.p ${name}.c main.c)
  add_dependencies(${name}.p genconfig feature)

  add_test(NAME prm_${name}
	   COMMAND ${testcmd} ${prmsrc}/${name}.sh ${prmobj}/${name}.p
	   WORKING_DIRECTORY ${srctree})
endforeach()

if(WIN32)
  fixup_runtime(${fake} ${OPENSSL_RUNTIME_LIB} ${SQLITE_RUNTIME_LIB})
endif()
