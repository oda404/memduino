cmake_minimum_required(VERSION 3.2)

project(memduinod)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "-O2 -Wall -Wextra -fstack-protector-all")

add_executable(${PROJECT_NAME}
    main.c
    memDuino.c
    memDuino.h
    serial.c
    serial.h
    exitcodes.h
    args.h
    args.c
)

set(link_libs "")

if(UNIX)
    set(link_libs ${link_libs} "m")
    add_custom_target(uninstall-daemon COMMAND ${CMAKE_SOURCE_DIR}/linux/uninstall-daemon.sh)
    add_custom_target(install-daemon COMMAND ${CMAKE_SOURCE_DIR}/linux/install-daemon.sh)
endif(UNIX)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${link_libs}
)

