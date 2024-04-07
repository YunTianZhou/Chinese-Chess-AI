#include "Player.h"
#include "tool.h"
#include <cmath>

Player::Player()
	: edge(50), spacing((WINDOW_HEIGHT - edge * 2) / (GRID_HEIGHT - 1)),
	  chess_board(nullptr), select_piece(nullptr),
	  last_move_begin_coordinate(nullptr),
	  last_move_end_coordinate(nullptr),
	  destination_set(nullptr), change(false)
{
	
}

Player::~Player()
{
	
}

void Player::Bind(CHESS_BOARD* chess_board,
	coordinate* select_piece,
	coordinate* last_move_begin_coordinate,
	coordinate* last_move_end_coordinate,
	CMA_RESULT_SET* destination_set)
{
	this->chess_board = chess_board;
	this->select_piece = select_piece;
	this->last_move_begin_coordinate = last_move_begin_coordinate;
	this->last_move_end_coordinate = last_move_end_coordinate;
	this->destination_set = destination_set;
}

bool Player::go(char color)
{
	change = false;

	int x, y;
	int half = PIECE_SIZE / 2;
	bool leftButton;
	tool::getMouseMessage(x, y, leftButton);

	if (!leftButton)
	{
		return false;
	}

	if (x >= edge - half and x <= WINDOW_WIDTH - edge + half and
		y >= edge - half and y <= WINDOW_HEIGHT - edge + half) {
		x -= edge;
		y -= edge;

		x = max(x, 0);
		y = max(y, 0);

		int gx = (int)std::round((float)x / spacing);
		int gy = (int)std::round((float)y / spacing);

		if (gx == select_piece->x and gy == select_piece->y) return false;

		if ((*chess_board)[gy][gx][0] == color)
		{
			select_piece->set(gx, gy);
			tool::getTargets({ gx, gy });
			tool::remove_check(*select_piece);
			change = true;
		}
		else if (destination_set->find({ gx, gy }) != destination_set->end())
		{
			last_move_begin_coordinate->set(select_piece->x, select_piece->y);
			select_piece->clear();
			last_move_end_coordinate->set(gx, gy);
			destination_set->clear();
			return true;
		}
	}

	return false;
}

bool Player::isChange()
{
	return change;
}
