cmake_minimum_required(VERSION 2.6)
SET(CMAKE_USE_RELATIVE_PATHS ON)
project(glimpse)

set(CFLAGS -g)
set(LDFLAGS \ )
#include directories
include_directories("include")
include_directories(".")

#build
aux_source_directory(src/ glimpse_FILES)
set_source_files_properties(${glimpse_FILES} PROPERTIES COMPILE_FLAGS ${CFLAGS} )
add_library(glimpse ${glimpse_FILES})

#api
macro(add_api prefix apiname)
	aux_source_directory(${prefix}/${apiname} api_FILES)
	set_source_files_properties(${api_FILES} PROPERTIES COMPILE_FLAGS ${CFLAGS}\ -I${prefix}/${apiname}/include)
	add_library(${apiname} ${api_FILES})
endmacro(add_api)

#test
macro(add_utest testname)
	set(__sources__ test/${testname}.c)
	set_source_files_properties(${__sources__} PROPERTIES COMPILE_FLAGS ${CFLAGS})
	add_executable(test/test_${testname} ${__sources__})
	target_link_libraries(test/test_${testname} glimpse)
	set_target_properties(test/test_${testname} PROPERTIES LINK_FLAGS ${LDFLAGS})
endmacro(add_utest)

#test
macro(add_utest_with_api testname api apidir)
	set(__sources__ test/${testname}.c)
	set_source_files_properties(${__sources__} PROPERTIES COMPILE_FLAGS ${CFLAGS}\ -I${apidir}/include)
	add_executable(test/test_${testname} ${__sources__})
	target_link_libraries(test/test_${testname} ${api})
	target_link_libraries(test/test_${testname} glimpse)
	target_link_libraries(test/test_${testname} dl)
	set_target_properties(test/test_${testname} PROPERTIES LINK_FLAGS ${LDFLAGS})
endmacro(add_utest_with_api)

add_utest(log)
add_api(test/api testapi)
add_utest_with_api(api testapi test/api/testapi)
