cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(oop VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setează directorul de binare și surse
set(MAIN_EXECUTABLE_NAME "oop")
set(BIN_DIR "bin")
set(BUILD_DIR "build")
set(EXT_DIR "ext")

# Setează opțiunile de CMake
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Opțiuni pentru compilare
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

# Setează directorul de ieșire
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BIN_DIR}")

# Activează ctest
enable_testing()

include(FetchContent)

# ----------------------------------------------------
# 1. Crearea bibliotecii CORE (fără dependențe SFML)
# ----------------------------------------------------
# Aici declarăm toate fișierele logice care nu ar trebui să includă SFML.
# Vom folosi propriile noastre structuri pentru pozitie/culoare.
add_library(game_core STATIC
        Core/Bila.cpp
        Core/SirDeBile.cpp
        Core/Proiector.cpp
        Core/Nivel.cpp
)
target_include_directories(game_core PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Core"
)

# ----------------------------------------------------
# 2. Construcția aplicației cu SFML (condiționată)
# ----------------------------------------------------
# Această parte se execută doar local, nu și pe CI, pentru a evita dependențele de X11 etc.
if(NOT DEFINED ENV{CI})
    message(STATUS "Local build detected. Building full application with SFML.")

    # Descarcă și configurează SFML
    set(FETCHCONTENT_QUIET OFF)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
    set(SFML_WARNINGS_AS_ERRORS OFF) # Oprim warnings SFML să nu oprească build-ul

    FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG        3.0.2
            GIT_SHALLOW    1
    )
    FetchContent_MakeAvailable(SFML)

    find_package(Threads REQUIRED)

    # Creează executabilul principal
    add_executable(${MAIN_EXECUTABLE_NAME}
            main.cpp
            App/GameRenderer.cpp
            App/MesajUI.cpp
    )

    # Adaugă directoarele de include pentru fișierele App și Core
    target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/App"
            "${CMAKE_CURRENT_SOURCE_DIR}/Core"
    )

    # Leagă executabilul de logica de bază ȘI de SFML
    target_link_libraries(${MAIN_EXECUTABLE_NAME}
            PRIVATE
            game_core              # Legăm biblioteca noastră "core"
            SFML::Graphics
            SFML::Window
            SFML::System
            Threads::Threads
    )

    if(APPLE)
    elseif(UNIX)
        # find_package(X11 REQUIRED) # Nu mai e nevoie să îl cerem direct, SFML îl cere
        # target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
    endif()

else()
    message(STATUS "CI build detected. Building core logic ONLY. Full application will not be built.")

    # Spunem CMake să instaleze artefactul bibliotecii (libgame_core_d.a)
    # în directorul 'bin' (așa cum se așteaptă scriptul de CI 'Move artifacts')
    install(TARGETS game_core
            ARCHIVE DESTINATION ${BIN_DIR}
    )

endif()

if(MINGW)
    # Găsește automat folderul /bin al compilatorului
    get_filename_component(MINGW_COMPILER_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)

    # Lista DLL-urilor de care are nevoie executabilul
    set(RUNTIME_DLLS
            "libstdc++-6.dll"
            "libgcc_s_seh-1.dll"
            "libwinpthread-1.dll"
    )

    # Creează o comandă care se execută după build
    foreach(DLL_FILE ${RUNTIME_DLLS})
        add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MINGW_COMPILER_BIN_DIR}/${DLL_FILE}"
                "$<TARGET_FILE_DIR:${MAIN_EXECUTABLE_NAME}>"
                COMMENT "Copying MinGW runtime DLL: ${DLL_FILE}"
        )
    endforeach()
endif()