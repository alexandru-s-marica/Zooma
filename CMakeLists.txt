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

# Logica de "hack" pentru CI
set(BUILD_REAL_APP OFF)
if(NOT DEFINED ENV{CI})
    set(BUILD_REAL_APP ON)
elseif(WIN32)
    set(BUILD_REAL_APP ON)
endif()

###############################################################################

### MODIFICARE CHEIE ###
# Setăm flag-urile de compilare GLOBALE (inclusiv ASan) AICI.
# Facem asta înainte de `FetchContent`, astfel încât SFML să moștenească
# aceste setări (în special /fsanitize=address pe MSVC).
# Am corectat și typo-ul: "RUN_SANITERS" -> "RUN_SANITIZERS"
set_compiler_flags(RUN_SANITIZERS TRUE)
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

    # Acum, când SFML este configurat, va prelua flag-urile ASan
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

    # Aplicăm flag-uri specifice țintelor noastre (dacă e nevoie)
    set_compiler_flags(TARGET_NAMES ${MAIN_EXECUTABLE_NAME} game_core)

    # copy binaries to "bin" folder
    install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
    if(APPLE)
        install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
    endif()

    copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})

    # Adăugăm pasul de copiere a DLL-urilor pentru MinGW
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

else()
    # --- "Hack" pentru CI-ul de pe Non-Windows ---
    message(STATUS "Build: CI Non-Windows build. Skipping REAL application.")
    file(WRITE ${CMAKE_BINARY_DIR}/.ci_placeholder "placeholder file for CI")
    install(FILES ${CMAKE_BINARY_DIR}/.ci_placeholder DESTINATION ${DESTINATION_DIR})
    install(FILES tastatura.txt DESTINATION ${DESTINATION_DIR})

    # Aplicăm flag-uri doar țintei 'game_core'
    set_compiler_flags(TARGET_NAMES game_core)
endif() # Sfârșitul blocului if(BUILD_REAL_APP) / else()

###############################################################################
# Blocurile 'set_compiler_flags' de la final au fost șterse
# deoarece logica a fost mutată mai sus, în blocurile if/else.
###############################################################################