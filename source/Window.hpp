#pragma once

#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

class Window
{
public:
    Window();
    ~Window();

    // Get window ref
    SDL_Window* get() const { return this->window_; }

    // Get renderer ref
    SDL_Renderer* getRenderer() const { return this->renderer_; }

private:
    // Window setting flag
    SDL_WindowFlags flag_;

    // Window ref
    SDL_Window* window_;

    // Renderer ref
    SDL_Renderer* renderer_;

    // Background color
    ImVec4 bg_color_;
};
