#include "Window.hpp"

Window::Window()
{
    // Create window
    this->flag_ = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    this->window_ = SDL_CreateWindow("NodeImageProcessing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, this->flag_);

    // Create renderer
    this->renderer_ = SDL_CreateRenderer(this->window_, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // Set background color
    this->bg_color_ = ImVec4(0.6f, 0.6f, 0.6f, 1.f);
}

Window::~Window()
{
    // Clean up SDL
    SDL_DestroyRenderer(this->renderer_);
    SDL_DestroyWindow(this->window_);
}
