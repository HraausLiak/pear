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
    sys/thread.h
    
    util/string.h
    util/string.cc
    util/time.h
    util/time.cc
    
    net/server_socket.h
    net/server_socket.cc
    net/server_socket_events.h
    net/thread.h
    net/thread.cc
    net/socket.h
    net/socket.cc
    net/tcp_server.h
    net/tcp_server.cc
    net/socket_factory.h
)

if (WIN32)
set(plat_SRC
    sys/thread_win.h
    sys/thread_win.cc
)
else()
endif()

add_library(public STATIC ${public_SRC} ${plat_SRC})