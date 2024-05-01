#include <cstdio>
#include <utility>
#include <iostream>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Scenes.hpp"
#include "Image.hpp"
#include "ImageSaver.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "STL.hpp"
#include "Ray.hpp"
#include "Vec3D.hpp"
#include "Shapes.hpp"
#include "Material.hpp"
#include "Utills.hpp"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif


namespace {
    // The SDL_Surface must die before im
    SDL_Surface* from_image(std::vector<char>& im, int WindowWidth, int WindowHeight) {
        return SDL_CreateRGBSurfaceFrom(im.data(),
            WindowWidth,
            WindowHeight,
            3 * 8,          // bits per pixel = 24
            WindowWidth * 3,  // pitch
            0x0000FF,              // red mask
            0x00FF00,              // green mask
            0xFF0000,              // blue mask
            0);
    }

    // Maybe look at using a variation of:
    // https://gamedev.stackexchange.com/questions/135126/in-sdl2-what-is-the-fastest-most-efficient-way-to-draw-pixels-onto-the-screen
    // To speed this up. I could make a texture at startup, and update it as needed
    void RTRender(CMRenderer& ray_renderer, std::vector<char>& buffer, SDL_Renderer* renderer, int WindowWidth, int WindowHeight) {
        ray_renderer.get_rgb_buffer(buffer);

        SDL_Surface* loaded_surface = from_image(buffer, WindowWidth, WindowHeight);

        SDL_Texture* loaded_tex = SDL_CreateTextureFromSurface(renderer, loaded_surface);

        if (loaded_tex == NULL)
        {
            std::cout << "SDL_CreateTextureFromSurface: " << SDL_GetError();
            std::exit(-1);
        }

        SDL_Rect texture_rect = {};
        texture_rect.x = 0;            // the x coordinate
        texture_rect.y = 0;            // the y coordinate
        texture_rect.w = WindowWidth;  // the width of the texture
        texture_rect.h = WindowHeight; // the height of the texture

        if (SDL_RenderCopy(renderer, loaded_tex, NULL, &texture_rect) != 0)
        {
            std::cout << "SDL_RenderCopy: " << SDL_GetError();
            std::exit(-1);
        }

        SDL_DestroyTexture(loaded_tex);
        SDL_FreeSurface(loaded_surface);
    }
}

// Main code
int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d12");

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI);
    const int WindowWidth = 1280;
    const int WindowHeight = 720;

    HittableScene world{ emissionTest() };
    Camera cam(Vec3D(13, 2, 3), Vec3D(-13, -2, -3), Vec3D(0, -1, 0), 10, WindowWidth, WindowHeight);
    int num_render_threads = std::max(static_cast<int>(std::thread::hardware_concurrency()) - 2, 1);
    num_render_threads = 0;
    CMRenderer ray_renderer(num_render_threads, cam, world);

    SDL_Window* window = SDL_CreateWindow("Raytracing In A Weekend - William Mosier", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }
    // SDL_RendererInfo info;
    // SDL_GetRendererInfo(renderer, &info);
    // SDL_Log("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Our state
    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    std::vector<char> tmp_buffer;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        bool save_image_flag = false;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            {
                done = true;
            }

            if (event.type == SDL_KEYDOWN) {
                // Handle key presses here
                switch (event.key.keysym.sym) {

                case SDLK_SPACE:
                    save_image_flag = true;
                    break;
                default:
                    break;
                }
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Options"); // Create a window called "Hello, world!" and append into it.
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);

        // Render Raytraced Image
        RTRender(ray_renderer, tmp_buffer, renderer, WindowWidth, WindowHeight);

        // Save image if space is pressed
        if (save_image_flag) [[unlikely]]
        {
            using namespace std::chrono;
            milliseconds ms = duration_cast<milliseconds>(
                system_clock::now().time_since_epoch()
            );
            std::string file_path = std::to_string(ms.count()) + ".bmp";
            BMPImageSaver::save(ray_renderer.get_image(), file_path.c_str());
        }

        // End Render Raytraced Image

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}