cmake_minimum_required(VERSION 3.2)

project(memduinod)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "-O2 -Wall -Wextra -fstack-protector-all")

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/argx argx)

set(INCLUDE_DIR include/memduinod)
set(SRC_DIR src)

set(SRC
    ${SRC_DIR}/main.c
    ${SRC_DIR}/memduinod.c
    ${SRC_DIR}/serial.c
    ${SRC_DIR}/meminfo.c
)

set(HEADERS
    ${INCLUDE_DIR}/exitcodes.h
    ${INCLUDE_DIR}/memduinod.h
    ${INCLUDE_DIR}/serial.h
    ${INCLUDE_DIR}/meminfo.h
)

add_executable(${PROJECT_NAME}
    ${SRC}
    ${HEADERS}
)

set(link_libs "argx")

if(UNIX)
    set(link_libs ${link_libs} "m")
    add_custom_target(uninstall-daemon COMMAND ${CMAKE_SOURCE_DIR}/linux/uninstall-daemon.sh)
    add_custom_target(install-daemon COMMAND ${CMAKE_SOURCE_DIR}/linux/install-daemon.sh)
endif(UNIX)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${link_libs}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include/
)
