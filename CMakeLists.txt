cmake_minimum_required(VERSION 3.2)

project(memDuino)

set(CMAKE_C_FLAGS "-O2 -Wall -Wextra")

add_executable(${PROJECT_NAME}
    src/c/main.c
    src/c/memDuino.c
    src/c/memDuino.h
    src/c/serial.c
    src/c/serial.h
)

if(UNIX)
    install(CODE "file(COPY memDuino DESTINATION /usr/bin)")
endif(UNIX)

target_link_libraries(${PROJECT_NAME} PRIVATE
    m
)