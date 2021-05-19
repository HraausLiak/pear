## public
cmake_minimum_required(VERSION 2.6)

project(public)

include_directories(
    ${PROJECT_SOURCE_DIR}
)

set(public_SRC
    base/application.h
    base/application.cc
    base/exception.h
    base/option.h
    base/option.cc
    base/option_set.h
    base/option_set.cc
)

add_library(public STATIC ${public_SRC})