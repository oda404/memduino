cmake_minimum_required(VERSION 3.2)

project(memDuino)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "-O2 -Wall -Wextra")

add_executable(${PROJECT_NAME}
    src/c/main.c
    src/c/memDuino.c
    src/c/memDuino.h
    src/c/serial.c
    src/c/serial.h
    src/c/exitcodes.h
)

set(link_libs "")

if(UNIX)
    install(CODE "execute_process(COMMAND ../src/linux/install.sh)")
    set(link_libs ${link_libs} "m")
endif(UNIX)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${link_libs}
)