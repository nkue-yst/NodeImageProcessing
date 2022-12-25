#include <iostream>
#include <memory>

#include "SDL.h"
#include "SDL_opengl.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"

#include "NodeBase.hpp"
#include "NodeEditor.hpp"

int main(int argc, char **argv)
{
    // Initialize SDL system
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    const char* glsl_version = "#version 130";

    // Create window
    int32_t win_width  = 1280;
    int32_t win_height = 720;
    SDL_WindowFlags win_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    //SDL_Window *win = SDL_CreateWindow("NodeImageProcessing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, win_flags);
    SDL_Window *win = SDL_CreateWindow("NodeImageProcessing", 100, 1500, win_width, win_height, win_flags);

    SDL_GLContext gl_context = SDL_GL_CreateContext(win);
    SDL_GL_MakeCurrent(win, gl_context);
    SDL_GL_SetSwapInterval(1);

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGuiContext *imgui_context = ImGui::CreateContext();
    ImNodesContext *imnodes_context = ImNodes::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(win, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Initialize Node editor
    std::unique_ptr<class NodeEditor> node_editor(new NodeEditor());
    node_editor->init();

    // Font settings
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->Build();

    // Set style
    ImGui::StyleColorsDark();
    ImVec4 bg_color = ImVec4(0.4f, 0.4f, 0.4f, 1.f);

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
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Begin drawing window
        ImGui::SetNextWindowSize(ImVec2(win_width, win_height), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));

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
                if (ImGui::MenuItem("Open Image File"))
                {
                    std::cout << "Selected \"Open image file\"" << std::endl;
                }

                // Exit button
                if (ImGui::MenuItem("Exit"))
                {
                    done = true;
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Edit menu
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("ImageProcessing"))
            {
                // Create new ImageNode
                if (ImGui::MenuItem("ImageSource"))
                {
                    node_editor->newImageNode(NT_ImageSource);
                }

                // Create new BinariztionNode
                if (ImGui::MenuItem("Binarization"))
                {
                    node_editor->newImageNode(NT_Binarization);
                }

                // Create new GrayScalingNode
                if (ImGui::MenuItem("GrayScaling"))
                {
                    node_editor->newImageNode(NT_GrayScaling);
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        //////////////////////
        ///// Draw nodes /////
        //////////////////////
        node_editor->draw();

        // End drawing window
        ImGui::End();

        /////////////////////
        ///// Rendering /////
        /////////////////////
        ImGui::Render();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(
            bg_color.x * bg_color.w,
            bg_color.y * bg_color.w,
            bg_color.z * bg_color.w,
            bg_color.w
        );
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(win);
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    // Clean up ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    ImNodes::DestroyContext();
    ImGui::DestroyContext();

    // Quit SDL system
    SDL_Quit();
}
