cmake_minimum_required(VERSION 3.2)

project(memDuino)

set(CMAKE_C_FLAGS "-O2 -Wall")

add_executable(${PROJECT_NAME}
    src/c/main.c
    src/c/memDuino.c
    src/c/memDuino.h
    src/c/serial.c
    src/c/serial.h
)

set(link_libs "")

if(UNIX)
    install(CODE "file(COPY memDuino DESTINATION /usr/bin)")
    set(link_libs ${link_libs} "m")
endif(UNIX)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${link_libs}
)