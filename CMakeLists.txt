cmake_minimum_required(VERSION 3.24)

project(mcpl_filter LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(mcpl)

add_executable(mcpl_filter "${PROJECT_SOURCE_DIR}/mcpl_filter.c")
target_link_libraries(mcpl_filter MCPL::mcpl)

install(TARGETS mcpl_filter DESTINATION bin)

