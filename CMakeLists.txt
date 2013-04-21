cmake_minimum_required(VERSION 2.6)
project(glimpse)
set(glimpse_log_level 3)
set(CFLAGS -g\ -DLOG_LEVEL=${glimpse_log_level}\ -DENABLE_PROFILER)
set(LDFLAGS \ )

include(utils.cmake)

#build

#libglimpse.a
aux_source_directory(src/ glimpse_FILES)
set_source_files_properties(${glimpse_FILES} PROPERTIES COMPILE_FLAGS ${CFLAGS} )
add_library(glimpse ${glimpse_FILES})
#type api
add_api(api TypeAPI .)


#glimpse-cli
set_source_files_properties("utils/cli.c" PROPERTIES COMPILE_FLAGS ${CFLAGS}\ -Iapi/TypeAPI/include)
add_executable(glimpse-cli "utils/cli.c")
target_link_libraries(glimpse-cli readline)
target_link_libraries(glimpse-cli TypeAPI)
target_link_libraries(glimpse-cli testapi)
target_link_libraries(glimpse-cli glimpse)
target_link_libraries(glimpse-cli dl)

add_plugin(test/plugin testapi_plugin test/api/testapi test)
add_utest(log)
add_api(test/api testapi test)
add_utest_with_api(api testapi test/api/testapi test)
add_utest_with_api(plugin testapi test/api/testapi test)
add_plugin(test/plugin deptest_A test/api/testapi test)
add_plugin(test/plugin deptest_B test/api/testapi test)
add_plugin(test/plugin deptest_C test/api/testapi test)
add_plugin(test/plugin loopdep_A test/api/testapi test)
add_plugin(test/plugin loopdep_B test/api/testapi test)
add_plugin(test/plugin baddep_A test/api/testapi test)
add_plugin(test/plugin baddep_B test/api/testapi test)
add_utest_with_api(plugindep testapi test/api/testapi) 
add_plugin(plugin integer api/TypeAPI .)
add_utest_with_api(integer TypeAPI api/TypeAPI)
add_utest(symbol)
add_plugin(test/plugin export api/TypeAPI test)
add_plugin(test/plugin import api/TypeAPI test)
add_utest_with_api(typeapi TypeAPI api/TypeAPI)
add_utest(chartable)
add_utest(vector)
add_utest_with_api(typesystem TypeAPI api/TypeAPI)
add_utest_with_api(scanner TypeAPI api/TypeAPI)
add_utest(profiler)
add_utest(strpool)
add_utest_with_api(typeparser TypeAPI api/TypeAPI)
