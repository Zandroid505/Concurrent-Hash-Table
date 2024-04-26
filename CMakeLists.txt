cmake_minimum_required(VERSION 3.0.0)
project(chash VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(chash chash.c file_io.c hashdb.c rwlocks.c)

# Copy input file into build directoy for execution.
configure_file(${CMAKE_SOURCE_DIR}/commands.txt ${CMAKE_BINARY_DIR}/commands.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/commands1.txt ${CMAKE_BINARY_DIR}/commands1.txt COPYONLY)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(chash PRIVATE Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
