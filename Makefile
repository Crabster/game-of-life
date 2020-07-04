build:
	clang++-10 `sdl2-config --cflags --libs` main.cpp -o gol
