#include <iostream>

#include "grid.h"

const int WIDTH = 40;
const int HEIGHT = 40;
const int SQUARE_SIDE = 20;

int main() {
    Grid<HEIGHT, WIDTH, SQUARE_SIDE> grid;
    grid.play();
    return 0;
}




