cmake_minimum_required(VERSION 3.0)

project(MINECRAFT)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Typ sestaveni" FORCE)
endif()

add_executable(MINECRAFT
    main.cpp
    types.cpp
    protocol.cpp
    network.cpp
    users.cpp
    game.cpp
)

install(TARGETS MINECRAFT RUNTIME DESTINATION bin)
