cmake_minimum_required(VERSION 3.2)

project(memduinod)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "-O2 -Wall -Wextra -Werror=format-security -fstack-protector-all -D_FORTIFY_SOURCE=2")

add_executable(${PROJECT_NAME}
    main.c
    memDuino.c
    memDuino.h
    serial.c
    serial.h
    exitcodes.h
)

set(link_libs "")

if(UNIX)
    set(link_libs ${link_libs} "m")
endif(UNIX)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${link_libs}
)

