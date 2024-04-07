#pragma once
#include "Player.h"
#include "Ai.h"
#include "Renderer.h"
#include "tool.h"
#include "button.h"
#include "radio_button.h"
#include "slider.h"
#include "PublicResource.h"
#include "FileHelper.h"

class Game
{
private:
	void reset_game_info();
	void new_record();
	void save_record();
	bool renew_game(char winner);
	void view_final();
	void review_reset_info(int& ptr, bool right);
	void reget();
	void deduce();
	void deduce_reget();
public:
	Game(Renderer* other_renderer);
	~Game();
	void Bind(std::vector<Record>* recent_record_list,
		std::vector<Record>* my_record_list,
		std::vector<Record>* load_record_list, 
		std::vector<std::pair<Record, Record>>* linked_record);
	void start();
	void review();
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
	float not_eat_times;

	std::vector<Record>* recent_record_list;
	std::vector<Record>* my_record_list;
	std::vector<Record>* load_record_list;
	std::vector<std::pair<Record, Record>>* linked_record;
};

