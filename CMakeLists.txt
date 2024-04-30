cmake_minimum_required(VERSION 3.18.0)
project(RaytracingInAWeekend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # SDL2
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC_PIC ON CACHE BOOL "" FORCE)
# set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
add_subdirectory(lib/SDL2)

# # SDL2_Image
add_compile_definitions(LOAD_BMP, SDL_IMAGE_USE_COMMON_BACKEND)
set(SDL2_IMAGE_SRC_FILES
    lib/SDL2_Image/src/IMG.c
    lib/SDL2_Image/src/IMG_WIC.c
    lib/SDL2_Image/src/IMG_avif.c
    lib/SDL2_Image/src/IMG_bmp.c
    lib/SDL2_Image/src/IMG_gif.c
    lib/SDL2_Image/src/IMG_jpg.c
    lib/SDL2_Image/src/IMG_jxl.c
    lib/SDL2_Image/src/IMG_lbm.c
    lib/SDL2_Image/src/IMG_pcx.c
    lib/SDL2_Image/src/IMG_png.c
    lib/SDL2_Image/src/IMG_pnm.c
    lib/SDL2_Image/src/IMG_qoi.c
    lib/SDL2_Image/src/IMG_stb.c
    lib/SDL2_Image/src/IMG_svg.c
    lib/SDL2_Image/src/IMG_tga.c
    lib/SDL2_Image/src/IMG_tif.c
    lib/SDL2_Image/src/IMG_webp.c
    lib/SDL2_Image/src/IMG_xcf.c
    lib/SDL2_Image/src/IMG_xpm.c
    lib/SDL2_Image/src/IMG_xv.c
)



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
    ${IMGUI_SRC_FILES}
    ${SDL2_IMAGE_SRC_FILES}
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
    "src/ThreadPool.cpp" "src/ThreadPool.cpp")

# Add IMGui Src Files
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/imgui)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/imgui/backends)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/Eigen)

# SDL_Image Src Files
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/SDL2_Image/include)

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

# target_link_libraries(${CMAKE_PROJECT_NAME} SDL2main SDL2-static)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2)