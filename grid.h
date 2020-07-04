#pragma once

#include <chrono>
#include <vector>
#include <array>

#include "sdl_handler.h"
#include "config.h"

struct Point {                                                                                   
    int x;
    int y;
};


template <int HEIGHT, int WIDTH, int SQUARE_SIZE>
class Grid {
private:
    using Board = std::array<std::array<bool, HEIGHT>, WIDTH>;

    SDL_Handler<HEIGHT, WIDTH, SQUARE_SIZE> sdlHandler;
    Board board{};
    bool running = false;

    std::chrono::milliseconds timeNow() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
        );
    }

    bool is_valid_point(Point p) {
        return 0 <= p.x && p.x < WIDTH && 0 <= p.y && p.y < HEIGHT;
    }

public:
    void updateRender() {
        sdlHandler.renderClear();
        sdlHandler.renderBoard(board);
        sdlHandler.renderUpdate();
    }

    bool action(SDL_Event e) {
        unsigned long points;
        switch(e.key.keysym.sym) {
            case SDLK_RETURN:
                running = !running;
                break;

            case SDLK_q:
                return true;

            default:
                break;
        }
        return false;
    }

    void process_mouse_click() {
        int m_x, m_y;
        SDL_GetMouseState(&m_x, &m_y);
        int x = m_x / (SQUARE_SIZE + 1);
        int y = m_y / (SQUARE_SIZE + 1);
        board[x][y] = !board[x][y];
    }

    std::vector<Point> cell_neighbours(Point cell) {
        std::vector<Point> neighbours;
        for (int x = cell.x - 1; x <= cell.x + 1; ++x) {
            for (int y = cell.y - 1; y <= cell.y + 1; ++y) {
                if (is_valid_point({x, y}) && (x != cell.x || y != cell.y)) {
                    neighbours.push_back({x, y});
                }
            }
        }
        return neighbours;            
    }

    int get_alive_cells_count(const std::vector<Point>& neighbours, const Board& old_board) {
        int alive_neighbours_count = 0;
        for (const Point& cell : neighbours) {
            if (old_board[cell.x][cell.y]) {
                ++alive_neighbours_count;
            }
        }
        return alive_neighbours_count;
    }

    bool should_cell_live(const Point& cell, const Board& old_board) {
        std::vector<Point> neighbours = cell_neighbours(cell);
        int alive_neighbours = get_alive_cells_count(neighbours, old_board);

        if (!old_board[cell.x][cell.y]) {
            return alive_neighbours == 3;
        }

        return 1 < alive_neighbours && alive_neighbours < 4;
    }

    void updateBoard() {
        Board old_board = board;  
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                board[x][y] = should_cell_live({x, y}, old_board);
            }
        }
    }

    void play() {
        bool quit = false;
        SDL_Event e;
        long long lastUpdateTime = timeNow().count() - Config::GAMETICK - 1;
        unsigned long points = 0;

        while(!quit) {
            while (SDL_PollEvent(&e) != 0) {
                switch (e.type) {
                    case SDL_QUIT:
                        quit = true;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        process_mouse_click();
                        break;
                    case SDL_KEYDOWN:
                        quit = action(e);
                        break;
                    default:
                        break;
                }
            }

            if (running) {
                if (lastUpdateTime + Config::GAMETICK < timeNow().count()) {
                    lastUpdateTime = timeNow().count();
                    updateBoard();
                }
            }

            updateRender();
        }
    }
};
