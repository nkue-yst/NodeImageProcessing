#include <memory>

#include "Window.hpp"

#include <SDL.h>

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    // Window instance
    std::unique_ptr<Window> win = std::make_unique<Window>();

    // Initialize ImGui
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(win->get(), win->getRenderer());
    ImGui_ImplSDLRenderer_Init(win->getRenderer());

    bool show_window = true;
    bool show_another_window = false;

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
            else if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE && ev.window.windowID == SDL_GetWindowID(win->get()))
            {
                done = true;
            }
        }
    }

    // Clean up ImGui
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_Quit();
}
