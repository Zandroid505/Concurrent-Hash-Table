cmake_minimum_required(VERSION 3.0.0)
project(chash VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(chash chash.c file_io.c hashdb.c)

# Copy input file into build directoy for execution.
configure_file(${CMAKE_SOURCE_DIR}/commands.txt ${CMAKE_BINARY_DIR}/commands.txt COPYONLY)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
