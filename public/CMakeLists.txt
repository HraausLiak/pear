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
    
    log/log.h
    log/log.cc
    log/log_file.h
    log/log_file.cc
    log/log_core.h
    log/log_core.cc
    
    sys/process.h
    sys/process.cc
    sys/file.h
    sys/file.cc
    
    util/string.h
    util/string.cc
    util/time.h
    util/time.cc
)

add_library(public STATIC ${public_SRC})