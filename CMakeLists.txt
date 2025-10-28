cmake_minimum_required(VERSION 3.23)
project(oop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

add_executable(oop
        main.cpp
        Bila.cpp
        Proiector.cpp
        SirDeBile.cpp
        Nivel.cpp
)

install(TARGETS oop RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})