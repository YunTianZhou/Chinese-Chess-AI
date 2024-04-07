#pragma once
#include "Headers.h"
class Ai
{
public:
	Ai();
	~Ai();
	void Bind(CHESS_BOARD* chess_board,
		coordinate* last_move_begin_coordinate,
		coordinate* last_move_end_coordinate,
		int* keepCheckTimes, 
		std::unordered_set<std::string>* keepCheckRecord);
	bool go(int first);
	int MiniMax(int depth, bool first, bool blackMove, int alpha, int beta);
	int searchAllCaptures(bool blackMove, int alpha, int beta);
	int scoring(bool blackMove);
	int scoring(int x, int y);
private:
	CHESS_BOARD* chess_board;
	coordinate* last_move_begin_coordinate;
	coordinate* last_move_end_coordinate;
	int* keepCheckTimes;
	std::unordered_set<std::string>* keepCheckRecord;
};

