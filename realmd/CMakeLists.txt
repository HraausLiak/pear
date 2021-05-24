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
    
    net/user_impl_socket.h
    net/user_impl_socket.cc
    net/user_impl_socket_factory.h
)

add_executable(realmd ${realmd_SRC})
target_link_libraries(realmd public)
if (WIN32)
target_link_libraries(realmd libevent ws2_32)
else()
endif()