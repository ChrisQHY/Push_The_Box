#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>

typedef enum {
	CELL_NULL = 0,
	CELL_WALL,
	CELL_PERSON,
	CELL_PERSON_WITH_TARGET,
	CELL_BOX_TARGET,
	CELL_BOX_SHIFT,
	CELL_BOX_COMPLITE,
}cell_type;



typedef enum {
	MOVE_W = 0,
	MOVE_S,
	MOVE_A,
	MOVE_D,
}move_dir;

typedef struct {
	char x;
	char y;
}person_pos;

class game
{
private:
	char row;
	char column;
	cell_type **map;
	person_pos p_pos;

public:
	game();
	~game();

	void draw_map();
	void action_map();
	bool finish_game();
	bool touch();
	bool move(move_dir dir, bool target_flag);
};

