cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(oop VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MAIN_EXECUTABLE_NAME "oop")
set(BIN_DIR "bin")
set(BUILD_DIR "build")
set(EXT_DIR "ext")

set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

option(WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
if (WARNINGS_AS_ERRORS)
    if (MSVC)
        add_compile_options(/W4 /WX)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    endif()
else()
    if (MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BIN_DIR}")

enable_testing()
include(FetchContent)

add_library(game_core STATIC
        Core/Bila.cpp
        Core/SirDeBile.cpp
        Core/Proiector.cpp
        Core/Nivel.cpp
        Core/utils.h
        Core/Bila.h
        Core/SirDeBile.h
        Core/Proiector.h
        Core/Nivel.h
)
target_include_directories(game_core PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Core"
)
install(TARGETS game_core ARCHIVE DESTINATION ${BIN_DIR})

set(BUILD_REAL_APP OFF)
if(NOT DEFINED ENV{CI})
    set(BUILD_REAL_APP ON)
elseif(WIN32)
    set(BUILD_REAL_APP ON)
endif()


if(BUILD_REAL_APP)
    set(FETCHCONTENT_QUIET OFF)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
    set(SFML_WARNINGS_AS_ERRORS OFF)

    FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG        3.0.2
            GIT_SHALLOW    1
    )
    FetchContent_MakeAvailable(SFML)
    find_package(Threads REQUIRED)

    add_executable(${MAIN_EXECUTABLE_NAME}
            main.cpp
            App/GameRenderer.cpp
            App/GameRenderer.h
            App/MesajUI.cpp
            App/MesajUI.h
    )

    target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/App"
            "${CMAKE_CURRENT_SOURCE_DIR}/Core"
    )

    target_link_libraries(${MAIN_EXECUTABLE_NAME}
            PRIVATE
            game_core
            SFML::Graphics
            SFML::Window
            SFML::System
            Threads::Threads
    )

    install(TARGETS ${MAIN_EXECUTABLE_NAME} RUNTIME DESTINATION ${BIN_DIR})

else()
    add_executable(${MAIN_EXECUTABLE_NAME}
            Core/mainsecundar.cpp
    )

    install(TARGETS ${MAIN_EXECUTABLE_NAME} RUNTIME DESTINATION ${BIN_DIR})

endif()

if(MINGW)
    get_filename_component(MINGW_COMPILER_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    set(RUNTIME_DLLS
            "libstdc++-6.dll"
            "libgcc_s_seh-1.dll"
            "libwinpthread-1.dll"
    )
    foreach(DLL_FILE ${RUNTIME_DLLS})
        add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MINGW_COMPILER_BIN_DIR}/${DLL_FILE}"
                "$<TARGET_FILE_DIR:${MAIN_EXECUTABLE_NAME}>"
        )
    endforeach()
endif()