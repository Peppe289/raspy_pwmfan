cmake_minimum_required(VERSION 3.30)
project(raspy_pwmfan C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)

set(CMAKE_C_STANDARD 23)

add_executable(raspy_pwmfan main.c
        header/header.h
        utils/sysfs.c)
target_link_libraries(raspy_pwmfan PkgConfig::GTK3)