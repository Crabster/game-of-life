#pragma once

#include <iostream>
#include <SDL.h>
#include <string>
#include <array>

class Window {
private:
    SDL_Window* window = nullptr;

public:
    Window(int width, int height) :
        window(SDL_CreateWindow("MLG Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)) {}

    ~Window() {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Window* operator()() const {
        return window;
    }
};

class Renderer {
private:
    SDL_Renderer* renderer = nullptr;

public:
    Renderer(const Window& window) :
        renderer(SDL_CreateRenderer(window(), -1, SDL_RENDERER_SOFTWARE)) {}

    ~Renderer() {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    SDL_Renderer* operator()() const {
        return renderer;
    }
};
