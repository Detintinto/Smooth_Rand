#include "Map.h"

Map* init_Map(int width, int height) {
	Map* map = new Map;
	map->hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	map->map_buffer = new CHAR_INFO[width*height];
	if (map->map_buffer == nullptr) {
		map->width = -1;
	}
	map->size.X = width;
	map->size.Y = height;

	map->window.Top = 0;
	map->window.Left = 0;
	map->window.Bottom = height - 1;
	map->window.Right = width - 1;

	map->width = width;
	map->height = height;

	SetConsoleWindowInfo(map->hOut, TRUE, &map->window);
	SetConsoleScreenBufferSize(map->hOut, map->size);

	return map;
}

void end_Map(Map* map) {
	
}

void print_Map(Map* map) {
	COORD coord;
	coord.X = 0;
	coord.Y = 0;
	WriteConsoleOutputA(map->hOut, map->map_buffer, map->size, coord, &map->window);
}