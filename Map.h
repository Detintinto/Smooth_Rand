#pragma once

#include <Windows.h> 
#include <stdlib.h>

class Map {
public:
	HANDLE hOut;
	CHAR_INFO* map_buffer;
	COORD size;
	SMALL_RECT window;

	int width;
	int height;
};

void end_Map(Map* map);
void print_Map(Map* map);
Map* init_Map(int width, int height);