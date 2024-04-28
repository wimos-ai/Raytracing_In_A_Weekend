cmake_minimum_required(VERSION 3.18.0)
project(RaytracingInAWeekend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/SDL2)




# Dear IMGui
set(IMGUI_SRC_FILES
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui.cpp
    lib/imgui/backends/imgui_impl_sdl2.cpp
    lib/imgui/backends/imgui_impl_sdlrenderer2.cpp
)

# src
add_executable(
    ${CMAKE_PROJECT_NAME}
    "./src/Interval.cpp"
    "./src/Utills.cpp"
    "./src/HittableScene.cpp"
    "./src/RGB_Pixel.cpp"
    "./src/Shapes.cpp"
    "./src/ImageSaver.cpp"
    "./src/main.cpp"
    "./src/Renderer.cpp"
    "./src/Camera.cpp"
    "./src/Image.cpp"
    "./src/Material.cpp"
    "./src/STLLoader.cpp"
    "./src/STL.cpp"
    "./src/Scenes.cpp"
    ${IMGUI_SRC_FILES}
    "src/ThreadPool.cpp" "src/ThreadPool.cpp")

# Add IMGui Src Files
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/imgui)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/imgui/backends)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/Eigen)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

if(supported)
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()

add_library(sse4_1 INTERFACE IMPORTED)
if(MSVC)
    message(STATUS "Loaded /arch:SSE4.1")
    target_compile_options(sse4_1 INTERFACE /arch:SSE4.1)
else()
    message(STATUS "Loaded -msse4.1")
    target_compile_options(sse4_1 INTERFACE -msse4.1)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} SDL2main SDL2-static)