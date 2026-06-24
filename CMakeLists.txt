cmake_minimum_required(VERSION 3.0)

project(MINECRAFT)

add_executable(MINECRAFT
    main.cpp
    types.cpp
    protocol.cpp
    network.cpp
    users.cpp
)

install(TARGETS MINECRAFT RUNTIME DESTINATION bin)
