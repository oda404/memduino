cmake_minimum_required(VERSION 3.2)

project(memduino_all)

add_subdirectory(daemon)

if(UNIX)
    add_custom_target(install-all COMMAND 
        make -C ${CMAKE_CURRENT_BINARY_DIR} install-daemon
    )
    add_custom_target(uninstall-all COMMAND 
        make -C ${CMAKE_CURRENT_BINARY_DIR} uninstall-daemon
    )
endif(UNIX)
