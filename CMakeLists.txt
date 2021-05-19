## pear
cmake_minimum_required(VERSION 2.6)

project(pear)

if (WIN32)
add_definitions(-D__WINDOWS__)
add_definitions(-D_WIN32_WINNT=0x0501)
add_definitions(-W3)
else()
add_definitions(-Wall)
    if (CMAKE_COMPILER_IS_GNUCXX)
        add_definitions(-std=gnu++0x)
    else ()
        add_definitions(-std=stdc++11)
    endif()
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}
)

add_subdirectory(public)
add_subdirectory(realmd)