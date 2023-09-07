cmake_minimum_required(VERSION 3.7...3.27)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Minidbg LANGUAGES CXX)

find_package(Boost REQUIRED)

include_directories(ext/libelfin ext/linenoise include)

add_custom_target(
   libelfin
   COMMAND make
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/libelfin
)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)