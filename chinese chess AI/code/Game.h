#pragma once
#include "Player.h"
#include "Ai.h"
#include "Renderer.h"
#include "tool.h"
#include "button.h"

class Game
{
private:
	void Init();
	void reSet();
	void new_record();
	void save_record();
	bool reNew(char winner);
	void view();
	void review();
	void review_reSetInfo(int& ptr, std::vector<std::string>& records, bool right);
public:
	Game();
	~Game();
private:
	Player* player;
	Ai* ai;
	Renderer* renderer;
	CHESS_BOARD chess_board;
	coordinate* select_piece;
	coordinate* last_move_begin_coordinate;
	coordinate* last_move_end_coordinate;
	CMA_RESULT_SET destination_set;
	char winner;
	std::wstring win_way;
	std::string game_record;
	int keepCheckTimes;
	std::unordered_set<std::string> keepCheckRecord;
};

