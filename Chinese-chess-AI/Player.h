#pragma once
#include "Headers.h"
class Player
{
public:
	Player();
	~Player();
	void Bind(CHESS_BOARD* chess_board,
		coordinate* select_piece,
		coordinate* last_move_begin_coordinate,
		coordinate* last_move_end_coordinate,
		CMA_RESULT_SET* destination_set);
	bool go(char color = 'r');
	bool isChange();
private:
	bool change;
	int edge;
	int spacing;
	CHESS_BOARD* chess_board;
	coordinate* select_piece;
	coordinate* last_move_begin_coordinate;
	coordinate* last_move_end_coordinate;
	CMA_RESULT_SET* destination_set;
};

