cmake_minimum_required(VERSION 3.26)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
# for now, the project name is used as the executable name
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})
# set(CMAKE_PROJECT_VERSION_MAJOR 0)
# set(CMAKE_PROJECT_VERSION_MINOR 0)
# set(CMAKE_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

# Adăugăm biblioteca de logică "core"
# Aceasta este compilată întotdeauna
add_library(game_core STATIC
        Core/Bila.cpp
        Core/SirDeBile.cpp
        Core/Proiector.cpp
        Core/Nivel.cpp
)
target_include_directories(game_core PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Core"
)

###############################################################################

### MODIFICARE ###
# Construim aplicația reală pe toate platformele pentru a rula testele
set(BUILD_REAL_APP ON)
###############################################################################

if(BUILD_REAL_APP)
    # --- CONSTRUCȚIA REALĂ (Local sau Windows CI) ---
    message(STATUS "Build: Building REAL application with SFML.")

    # external dependencies with FetchContent
    include(FetchContent)

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

    # NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
    add_executable(${MAIN_EXECUTABLE_NAME}
            main.cpp
            App/GameRenderer.cpp
            App/MesajUI.cpp
    )

    target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/App"
    )

    target_link_libraries(${MAIN_EXECUTABLE_NAME}
            PRIVATE
            game_core
            SFML::Graphics
            SFML::Window
            SFML::System
            Threads::Threads
    )

    # use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories
    target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
    target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)

    if(APPLE)
    elseif(UNIX)
        find_package(X11)
        if(X11_FOUND)
            target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
        endif()
    endif()

    # Blocurile de mai jos (install, copy_files, if MINGW) au fost mutate aici,
    # în interiorul lui if(BUILD_REAL_APP), deoarece toate depind de ținta
    # ${MAIN_EXECUTABLE_NAME} care se creează doar în acest bloc.

    # copy binaries to "bin" folder; these are uploaded as artifacts on each release
    # DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
    install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
    if(APPLE)
        install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
    endif()

    copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})

    # Adăugăm pasul de copiere a DLL-urilor pentru MinGW (Local sau CI)
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

endif() # Acesta este sfârșitul blocului if(BUILD_REAL_APP)

###############################################################################

# Aplicăm flag-urile de compilare condiționat.
# `game_core` există mereu, dar `${MAIN_EXECUTABLE_NAME}` (oop) există doar dacă BUILD_REAL_APP este ON.
if(BUILD_REAL_APP)
    # Aplicăm la ambele ținte
    set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME} game_core)
else()
    # Aplicăm doar la ținta care știm sigur că există
    set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES game_core)
endif()

###############################################################################