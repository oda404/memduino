cmake_minimum_required(VERSION 3.2)

project(memduino)

add_subdirectory(daemon)

if(UNIX)    
    add_custom_target(uninstall-daemon COMMAND "../linux/uninstall-daemon.sh")
    add_custom_target(install-daemon COMMAND "../linux/install-daemon.sh")
endif(UNIX)
