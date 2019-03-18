cmake_minimum_required(VERSION 3.2)
 
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/build)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)
SET(LIBEVENT_NAME "event" CACHE STRING "Name of the libevent library (as used in the linker argument).")
SET(LIBEVENT_LIB_DIR "/usr/local/lib" CACHE STRING "Path to libevent libraries.")
SET(LIBEVENT_INCLUDE_DIR "/usr/local/include" CACHE STRING "Path to libevent headers.")

add_executable(server "src/server/main.cpp" "src/server/util.cpp" "src/server/httpserv.h" "src/server/util.h")
target_link_libraries(server ${LIBEVENT_NAME})

add_executable(client "src/client/main.cpp" "src/client/util.cpp" "src/client/httpreq.h" "src/client/util.h")
target_link_libraries(client ${LIBEVENT_NAME})

add_executable(service "src/service/client_service.cpp")

link_directories("${LIBEVENT_LIB_DIR}/")

include_directories("${LIBEVENT_INCLUDE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

find_package(GTest REQUIRED)
if (GTEST_FOUND)
    enable_testing()
    add_custom_target(build_and_test ${CMAKE_CTEST_COMMAND} -V)
endif()

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(server_test "test/test_server.cpp" "src/server/util.cpp" "src/server/httpserv.h" "src/server/util.h")
target_link_libraries(server_test ${GTEST_LIBRARIES} pthread)
target_link_libraries(server_test ${LIBEVENT_NAME})

add_executable(client_test "test/test_client.cpp" "src/client/util.cpp" "src/client/httpreq.h" "src/client/util.h")
target_link_libraries(client_test ${GTEST_LIBRARIES} pthread)
target_link_libraries(client_test ${LIBEVENT_NAME})

add_test(server_test "build/server_test")
add_test(client_test "build/client_test")

add_dependencies(build_and_test server_test)
add_dependencies(build_and_test client_test)