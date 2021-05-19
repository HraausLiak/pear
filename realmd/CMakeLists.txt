## realmd
cmake_minimum_required(VERSION 2.6)

project(realmd)

include_directories(
    ${PROJECT_SOURCE_DIR}
)

set(realmd_SRC
    application.h
    application.cc
    main.cc
)

add_executable(realmd ${realmd_SRC})
target_link_libraries(realmd public)