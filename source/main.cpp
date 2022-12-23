#include <iostream>
#include <memory>
#include <vector>

#include "NodeBase.hpp"

#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "imnodes.h"

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    // Create window
    int32_t win_width  = 1280;
    int32_t win_height = 720;
    SDL_WindowFlags win_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    //SDL_Window *win = SDL_CreateWindow("NodeImageProcessing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, win_flags);
    SDL_Window *win = SDL_CreateWindow("NodeImageProcessing", 100, 1500, win_width, win_height, win_flags);

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGuiContext *imgui_context = ImGui::CreateContext();
    ImNodesContext *imnodes_context = ImNodes::CreateContext();

    ImGui_ImplSDL2_InitForSDLRenderer(win, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    // Font settings
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->Build();

    // Set style
    ImGui::StyleColorsDark();
    ImNodes::StyleColorsDark();
    ImVec4 bg_color = ImVec4(0.4f, 0.4f, 0.4f, 1.f);
    ImNodes::SetNodeGridSpacePos(1, ImVec2(200.f, 200.f));

    // Node list
    std::vector<class NodeBase*> node_list;

    bool done = false;
    while (!done)
    {
        // Process terminate input
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            ImGui_ImplSDL2_ProcessEvent(&ev);

            if (ev.type == SDL_QUIT)
            {
                done = true;
            }
            else if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE && ev.window.windowID == SDL_GetWindowID(win))
            {
                done = true;
            }
        }

        // Update main window size
        SDL_GetWindowSize(win, &win_width, &win_height);

        // Start ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Begin drawing node and window
        ImGui::SetNextWindowSize(ImVec2(win_width, win_height), ImGuiCond_Always);
        ImGui::Begin("NodeImageProcessing", nullptr, ImGuiWindowFlags_MenuBar);

        /////////////////////////
        ///// Draw menu bar /////
        /////////////////////////
        // File menu
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Open image file
                if (ImGui::MenuItem("Open"))
                {
                    std::cout << "Selected \"Open\"" << std::endl;
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Edit menu
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Edit"))
            {
                // Create new node
                if (ImGui::MenuItem("New node"))
                {
                    std::cout << "Selected \"New node\"" << std::endl;
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        //////////////////////
        ///// Draw nodes /////
        //////////////////////
        ImNodes::BeginNodeEditor();
        for (auto node : node_list)
        {
            node->draw();
        }

        // End drawing node and window
        ImNodes::EndNodeEditor();
        ImGui::End();

        ImGui::Render();

        // Rendering
        SDL_SetRenderDrawColor(
            renderer,
            bg_color.x * bg_color.w * 255,
            bg_color.y * bg_color.w * 255,
            bg_color.z * bg_color.w * 255,
            SDL_ALPHA_OPAQUE
        );
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    // Clean up ImGui
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImNodes::DestroyContext();
    ImGui::DestroyContext();
    SDL_Quit();
}
