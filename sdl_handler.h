#pragma once

#include "sdl_wrappers.h"


template <int HEIGHT, int WIDTH, int SQUARE_SIZE>
class SDL_Handler {
private:
    using Board = std::array<std::array<bool, HEIGHT>, WIDTH>;

    Window window;
    Renderer renderer;

    static void initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::string er("SDL could not initialize! SDL_Error: ", SDL_GetError());
            throw std::runtime_error(er);
        }
    }

public:
    SDL_Handler() :
        window((initSDL(), 1 + WIDTH * (1 + SQUARE_SIZE)), 1 + HEIGHT * (1 + SQUARE_SIZE)),
        renderer(window) {}

    ~SDL_Handler() {
        SDL_Quit();
    }

    void renderClear() {
        SDL_SetRenderDrawColor(renderer(), 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer());
    }

    void renderUpdate() {
        SDL_RenderPresent(renderer());
    }

    void renderBoard(const Board& board) {
        SDL_Rect rect;

        // Render playground screen
        rect = { 1, 1, (SQUARE_SIZE + 1) * WIDTH, (SQUARE_SIZE + 1) * HEIGHT };
        SDL_SetRenderDrawColor( renderer(), 0xC0, 0xC0, 0xC0, 0xC0 );
        SDL_RenderFillRect( renderer(), &rect );

        // Render board
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                rect.x = 1 + x * (SQUARE_SIZE + 1);
                rect.y = 1 + y * (SQUARE_SIZE + 1);
                rect.w = SQUARE_SIZE;
                rect.h = SQUARE_SIZE;
                if (board[x][y]) {
                    SDL_SetRenderDrawColor(renderer(), 0x00, 0x00, 0x00, 0x00);
                    SDL_RenderFillRect( renderer(), &rect );
                } else {
                    SDL_SetRenderDrawColor(renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderFillRect( renderer(), &rect );
                }
            }
        }
    }
};
