cmake_minimum_required(VERSION 3.16)
project(tap_gui_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- SDL2 (ventana + contexto OpenGL + eventos) ----------------------------
# Usa el SDL2 del sistema si existe (p.ej. el cluster de 42); si no, lo
# descarga y compila (p.ej. en Windows sin nada instalado).
find_package(SDL2 QUIET)
if (NOT SDL2_FOUND)
    message(STATUS "SDL2 no encontrado en el sistema, se descargara con FetchContent")
    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.9
    )
    FetchContent_MakeAvailable(SDL2)
else()
    message(STATUS "Usando SDL2 ya instalado en el sistema")
endif()

# --- Dear ImGui --------------------------------------------------------
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.9
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC SDL2::SDL2)

find_package(OpenGL REQUIRED)

# --- Cliente TAP ------------------------------------------------------
add_executable(tap_gui_client
    src/main.cpp
    src/GameClient.cpp
)

target_include_directories(tap_gui_client PRIVATE
    inc  # GameClient.hpp (ya no depende de ../inc, ver comentario en el propio archivo)
)

target_link_libraries(tap_gui_client PRIVATE imgui SDL2::SDL2 OpenGL::GL)

if (WIN32 AND TARGET SDL2::SDL2)
    add_custom_command(TARGET tap_gui_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:tap_gui_client>
    )
endif()