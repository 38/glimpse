cmake_minimum_required(VERSION 2.6)
SET(CMAKE_USE_RELATIVE_PATHS ON)
project(glimpse)

set(CFLAGS -g)

#include directories
include_directories("include")
include_directories(".")

#build
aux_source_directory(src/ glimpse_FILES)
set_source_files_properties(${glimpse_FILES} PROPERTIES COMPILE_FLAGS ${CFLAGS} )
add_library(glimpse ${glimpse_FILES})

#test
macro(add_utest testname)
	set(__sources__ test/${testname}.c)
	set_source_files_properties(${__sources__} PROPERTIES COMPILE_FLAGS ${CFLAGS})
	add_executable(test/test_${testname} ${__sources__})
	target_link_libraries(test/test_${testname} glimpse)
endmacro(add_utest)
add_utest(log)
