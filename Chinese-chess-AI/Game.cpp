#include "Game.h"

Game::Game(Renderer* other_renderer)
{
	player = new Player();
	ai = new Ai();
	renderer = other_renderer;

	chess_board = PublicResource::current_chessBoard;
	select_piece = new coordinate(-1, -1);
	last_move_begin_coordinate = new coordinate(-1, -1);
	last_move_end_coordinate = new coordinate(-1, -1);

	player->Bind(&chess_board, select_piece, last_move_begin_coordinate,
		last_move_end_coordinate, &destination_set);
	ai->Bind(&chess_board, last_move_begin_coordinate, last_move_end_coordinate,
		&keepCheckTimes, &keepCheckRecord);
	renderer->Bind(&chess_board, select_piece, last_move_begin_coordinate,
		last_move_end_coordinate, &destination_set);

	PublicResource::Cma = new cma::ChessMovesAnalyze<std::string>(chess_board);
	PublicResource::Cma->bind_result_set(destination_set);

	reset_game_info();
}

void Game::start()
{
	while (true) {

		bool first = PublicResource::current_go_first;

		renderer->draw_game_start_sign();
		while (true)
		{
			renderer->draw_game_with_batch(false);
			if (first)
			{
				bool tie = false;

				while (!player->go()) {
					if (player->isChange())
						renderer->draw_game_with_batch(false);
					if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
						if (GetAsyncKeyState('T') & 0x8000) {
							tie = true;
							break;
						}
						if (GetAsyncKeyState('Z') & 0x8000) {
							reget();
							renderer->draw_game_with_batch(false);
							Sleep(100);
						}
						if (GetAsyncKeyState('D') & 0x8000) {
							deduce();
							renderer->draw_game_with_batch(false);
							Sleep(100);
						}
					}
				}

				if (tie)
				{
					winner = 'n';
					win_way = _T("玩家求和");
					break;
				}

				first = true;

				new_record();
			}

			renderer->draw_game_with_batch(false);

			if (tool::isWin('r')) {
				winner = 'r';
				win_way = tool::isCheck('r') ? _T("玩家绝杀") : _T("电脑困毙");
				break;
			}

			if (tool::isCheck('r')) {
				renderer->draw_game_with_batch(true);
				Sleep(1000);
			}

			if (tool::is_tie(chess_board))
			{
				winner = 'n';
				win_way = _T("无子成杀");
				break;
			}

			if (not_eat_times >= not_eat_times_limit)
			{
				winner = 'n';
				win_way = _T("僵持不下");
				break;
			}
			else if (not_eat_times_limit - not_eat_times == 20.0f)
			{
				renderer->draw_message_box(_T("五十步未吃子和棋"));
				Sleep(1000);
				renderer->draw_game_with_batch(false);
				renderer->draw_message_box(_T("还剩二十步"));
				Sleep(1000);
				renderer->draw_game_with_batch(false);
			}

			renderer->draw_game_with_batch(false);

			bool respons = ai->go(first);

			if (respons)
			{
				winner = 'r';
				win_way = _T("对手长将");
				break;
			}

			new_record();

			renderer->draw_game_with_batch(false);

			if (tool::isWin('b')) {
				winner = 'b';
				win_way = tool::isCheck('b') ? _T("电脑绝杀") : _T("玩家困毙");
				break;
			}

			if (tool::isCheck('b')) {
				keepCheckTimes++;
				keepCheckRecord.insert(tool::chessboardToFanStr(chess_board));
				renderer->draw_game_with_batch(true);
				Sleep(1000);
			}
			else
			{
				keepCheckTimes = 0;
				keepCheckRecord.clear();
			}

			if (tool::is_tie(chess_board))
			{
				winner = 'n';
				win_way = _T("无子成杀");
				break;
			}

			if (not_eat_times >= not_eat_times_limit)
			{
				winner = 'n';
				win_way = _T("僵持不下");
				break;
			}
			else if (not_eat_times_limit - not_eat_times == 20.0f)
			{
				renderer->draw_message_box(_T("五十步未吃子和棋"));
				Sleep(1000);
				renderer->draw_game_with_batch(false);
				renderer->draw_message_box(_T("还剩二十步"));
				Sleep(1000);
				renderer->draw_game_with_batch(false);
			}

			first = true;
		}

		save_record();

		destination_set.clear();
		select_piece->clear();
		renderer->draw_game_final_sign(winner);
		Sleep(1000);

		if (renew_game(winner)) break;

		reset_game_info();
	}
	
}

Game::~Game()
{
	delete player;
	delete ai;
	delete select_piece;
	delete last_move_begin_coordinate;
	delete last_move_end_coordinate;
	delete PublicResource::Cma;
}

void Game::Bind(std::vector<Record>* recent_record_list,std::vector<Record>* my_record_list, std::vector<Record>* load_record_list, std::vector<std::pair<Record, Record>>* linked_record)
{
	this->recent_record_list = recent_record_list;
	this->my_record_list = my_record_list;
	this->load_record_list = load_record_list;
	this->linked_record = linked_record;
}

void Game::reset_game_info()
{
	chess_board = PublicResource::current_chessBoard;
	select_piece->clear();
	last_move_begin_coordinate->clear();
	last_move_end_coordinate->clear();
	destination_set.clear();
	winner = ' ';
	win_way = _T("");
	game_record = "------";
	keepCheckTimes = 0;
	keepCheckRecord.clear();
	not_eat_times = 0.0f;
}

void Game::new_record()
{
	// update record
	game_record += last_move_begin_coordinate->to_string() + 
		last_move_end_coordinate->to_string() + 
		chess_board[last_move_end_coordinate->y][last_move_end_coordinate->x];

	// Update not eat times
	if (chess_board[last_move_end_coordinate->y][last_move_end_coordinate->x] == "sp")
	{
		not_eat_times += 0.5f;
	}
	else
	{
		not_eat_times = 0;
	}
	
	// move piece
	chess_board[last_move_end_coordinate->y][last_move_end_coordinate->x] =
		chess_board[last_move_begin_coordinate->y][last_move_begin_coordinate->x];
	chess_board[last_move_begin_coordinate->y][last_move_begin_coordinate->x] = "sp";
}

void Game::save_record()
{
	// Save the info into curr_review_code.txt file
	FileHelper::writeFileSaveRecord(curr_review_code_txt_path, winner, win_way, game_record, 
		PublicResource::current_chessBoard, chess_board);

	// Create a new file in recent record list
	std::string recent_controlfile = recent_record_list_path + "/control.txt";
	std::string recent_filename = std::to_string(PublicResource::current_recent_id + 1) + ".txt";
	std::string recent_filepath = recent_record_list_path + "/" + recent_filename;
	std::ofstream recent_ofs(recent_filepath);
	recent_ofs.close();
	
	// Update the max number of recent record list
	PublicResource::current_recent_id++;

	// Write the info of the game in to the new file and get the Record obj
	Record recent_result = FileHelper::writeFileSaveRecord(recent_filepath, winner, win_way, game_record,
		PublicResource::current_chessBoard, chess_board);

	// Put the obj into the beginning of recent record list
	recent_record_list->insert(recent_record_list->begin(), recent_result);

	// Add the filename into the beginning control file 
	FileHelper::insertContentAtBeginning(recent_controlfile, recent_filename);

	// Get the lines of the control file ( each line is a game' filename )
	std::ifstream ifs(recent_controlfile);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(ifs, line))
	{
		lines.push_back(line);
	}
	ifs.close();

	// If number of the games greater than limit, cut the last game
	if (lines.size() > recent_record_limit)
	{
		// Delete the last game
		std::string last_game_filename = lines.back();
		std::string last_game_filepath = recent_record_list_path + "/" + last_game_filename;
		fs::remove(last_game_filepath);

		// Delete the last game's filename
		std::ofstream ofs(recent_controlfile);
		for (int i = 0; i < lines.size() - 1; i++)
		{
			ofs << lines[i] << std::endl;
		}
		ofs.close();

		// Check the last game was or was not linked
		bool is_linked = false; // a sigh express it's linked 
		std::string target; // The target line is if the last game was linked, the line in the file join.txt that we need to delete 
		for (int i = 0; i < linked_record->size(); i++)
		{
			if (linked_record->at(i).first == recent_record_list->back())
			{
				// If the last game was linked, unlink it
				std::string first = linked_record->at(i).first.path.substr(recent_record_list_path.size() + 1);
				std::string second = linked_record->at(i).second.path.substr(my_record_list_path.size() + 1);
				linked_record->erase(linked_record->begin() + i);
				target = first + "/" + second; // update the target
				is_linked = true; // update the sigh
				break;
			}
		}

		if (is_linked)
		{
			// If the last game was linked, delete the target line in the file join.txt
			FileHelper::removeLineInFile(recent_record_list_path + "/join.txt", target);
		}

		// Update the recent record list
		recent_record_list->pop_back();
	}

	// The logic of automatically add record in to my record list
	if (PublicResource::current_automatically_add)
	{
		// Create a new file in my record list
		std::string my_controlfile = my_record_list_path + "/control.txt";
		std::string my_filename = std::to_string(PublicResource::current_my_id + 1) + ".txt";
		std::string my_filepath = my_record_list_path + "/" + my_filename;
		std::ofstream my_ofs(my_filepath);
		my_ofs.close();

		// Update the max number of my record list
		PublicResource::current_my_id++;

		// Write the info of the game in to the new file and get the Record obj
		Record my_result = FileHelper::writeFileSaveRecord(my_filepath, winner, win_way, game_record,
			PublicResource::current_chessBoard, chess_board);

		// Put the obj into the beginning of my record list
		my_record_list->insert(my_record_list->begin(), my_result);

		// Add the filename into the beginning control file 
		FileHelper::insertContentAtBeginning(my_controlfile, my_filename);

		// Add a link in to join.txt ( format : recent-record.txt/my-record.txt )
		std::string joinfile = recent_record_list_path + "/join.txt";
		std::string link_code = recent_filename + "/" + my_filename;
		std::ofstream join_ofs(joinfile, std::ios_base::app); // append mode
		join_ofs << link_code << std::endl;
		join_ofs.close();

		// Update the linked-record ( Add the new link )
		linked_record->push_back({ recent_record_list->front(), my_record_list->front()});
	}
}

bool Game::renew_game(char winner)
{
	renderer->draw_game_renew(winner, win_way);
	Button renew(130, 500, 235, 70);
	Button checkEnd(375, 500, 235, 70);
	Button review(130, 580, 235, 70);
	Button exit(375, 580, 235, 70);
	while (true)
	{
		bool flip = false;
		MouseControl::update_mouse_message();
		if (renew.click_listen())
		{
			return false;
		} 
		else if (checkEnd.click_listen())
		{
			this->view_final();
			flip = true;
		}
		else if (review.click_listen())
		{
			this->review();
			flip = true;
		}
		else if (exit.click_listen())
		{
			return true;
		}

		if (flip)
			renderer->draw_game_renew(winner, win_way);
	}
	return false;
}

void Game::view_final()
{
	renderer->draw_game_with_batch(false);
	while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000));
}

void Game::review()
{
	CHESS_BOARD start_game_copy;
	CHESS_BOARD end_game_copy;
	coordinate last_move_begin_coordinate_copy(*last_move_begin_coordinate);
	coordinate last_move_end_coordinate_copy(*last_move_end_coordinate);
	bool go_first_copy = PublicResource::current_go_first;

	FileHelper::readFileGetRecordInfo(game_record, start_game_copy, end_game_copy, PublicResource::current_go_first);
	chess_board = start_game_copy;
	last_move_begin_coordinate->clear();
	last_move_end_coordinate->clear();
	renderer->draw_game_with_batch(false);

	int pointer = 0;
	bool right;
	int flip;
	while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
	{
		flip = false;
		if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000) {
			if (pointer >= 0)
			{
				right = false;
				flip = true;
			}
		}

		if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			if (pointer < 0 || pointer < game_record.size())
			{
				pointer += 6;
				right = true;
				flip = true;
			}
		}

		if (GetAsyncKeyState(VK_HOME) & 0x8000)
		{
			pointer = 0;
			chess_board = start_game_copy;
			last_move_begin_coordinate->clear();
			last_move_end_coordinate->clear();
			renderer->draw_game_record(pointer, game_record.size());
			Sleep(100);
		}

		if (GetAsyncKeyState(VK_END) & 0x8000)
		{

			pointer = (int)game_record.size() - 6;
			chess_board = end_game_copy;
			if (!pointer)
			{
				last_move_begin_coordinate->clear();
				last_move_end_coordinate->clear();
			}
			else
			{
				last_move_begin_coordinate->set(last_move_begin_coordinate_copy);
				last_move_end_coordinate->set(last_move_end_coordinate_copy);
			}
			renderer->draw_game_record(pointer, game_record.size());
			Sleep(100);
		}

		if (flip)
		{
			review_reset_info(pointer, right);
			renderer->draw_game_record(pointer, game_record.size());
			Sleep(100);
		}
	}

	chess_board = end_game_copy;
	last_move_begin_coordinate->set(last_move_begin_coordinate_copy);
	last_move_end_coordinate->set(last_move_end_coordinate_copy);
	PublicResource::current_go_first = go_first_copy;
}

void Game::review_reset_info(int& ptr, bool right)
{
	if (!right && (ptr == 0 || ptr >= game_record.size()))
	{
		ptr -= 6;
		return; 
	} 
	else if (right && (ptr == 0 || ptr >= game_record.size()))
	{
		return;
	}

	coordinate begin(game_record.substr(ptr, 2));
	coordinate end(game_record.substr(ptr + 2, 2));
	std::string record = game_record.substr(ptr + 4, 2);

	if (right)
	{
		chess_board[end.y][end.x] = chess_board[begin.y][begin.x];
		chess_board[begin.y][begin.x] = "sp";
	}
	else
	{
		chess_board[begin.y][begin.x] = chess_board[end.y][end.x];
		chess_board[end.y][end.x] = record;
		ptr -= 6;
		if (ptr == 0)
		{
			begin.clear();
			end.clear();
		}
		else 
		{
			begin.set(game_record.substr(ptr, 2));
			end.set(game_record.substr(ptr + 2, 2));
		}
	}

	last_move_begin_coordinate->set(begin);
	last_move_end_coordinate->set(end);
	select_piece->clear();
}

void Game::reget()
{
	// get imfo of last move
	int ptr = (int)game_record.size() - 12;
	if (ptr <= 0) return;
	coordinate player_start = { game_record.substr(ptr, 2) };
	coordinate player_end = { game_record.substr(ptr + 2, 2) };
	std::string player_last_move_target = game_record.substr(ptr + 4, 2);
	ptr += 6;
	coordinate ai_start = { game_record.substr(ptr, 2) };
	coordinate ai_end = { game_record.substr(ptr + 2, 2) };
	std::string ai_last_move_target = game_record.substr(ptr + 4, 2);

	//reset game_record ( cut 6 chars )
	game_record = game_record.substr(0, ptr - 6);
	
	// move back fllow the info 
	chess_board[ai_start.y][ai_start.x] = chess_board[ai_end.y][ai_end.x];
	chess_board[ai_end.y][ai_end.x] = ai_last_move_target;
	chess_board[player_start.y][player_start.x] = chess_board[player_end.y][player_end.x];
	chess_board[player_end.y][player_end.x] = player_last_move_target;

	// reset the last-move vars
	int length = (int)game_record.size();
	if (length > 6)
	{
		last_move_begin_coordinate->set(game_record.substr(length - 6, 2));
		last_move_end_coordinate->set(game_record.substr(length - 4, 2));
	}
	else
	{
		last_move_begin_coordinate->clear();
		last_move_end_coordinate->clear();
	}

	// claer the selects info	
	select_piece->clear();
	destination_set.clear();
}

void Game::deduce_reget()
{
	// get imfo of last move
	int ptr = (int)game_record.size() - 6;
	coordinate start_move = { game_record.substr(ptr, 2) };
	coordinate end_move = { game_record.substr(ptr + 2, 2) };
	std::string last_move_target = game_record.substr(ptr + 4, 2);

	// cut the game_record ( 6 chars )
	game_record = game_record.substr(0, ptr);

	// move the piece back to the prev position it at 
	chess_board[start_move.y][start_move.x] = chess_board[end_move.y][end_move.x];
	chess_board[end_move.y][end_move.x] = last_move_target;

	// reset the last-move vars
	int length = (int)game_record.size();
	if (length > 6)
	{
		last_move_begin_coordinate->set(game_record.substr(length - 6, 2));
		last_move_end_coordinate->set(game_record.substr(length - 4, 2));
	}
	else
	{
		last_move_begin_coordinate->clear();
		last_move_end_coordinate->clear();
	}

	// claer the selects info	
	select_piece->clear();
	destination_set.clear();
}

void Game::deduce()
{
	CHESS_BOARD chess_board_copy = chess_board;
	coordinate select_piece_copy(*select_piece);
	coordinate last_move_begin_coordinate_copy(*last_move_begin_coordinate);
	coordinate last_move_end_coordinate_copy(*last_move_end_coordinate);
	CMA_RESULT_SET destination_set_copy = destination_set;
	std::string game_record_copy = game_record;

	char who = 'r';
	bool end = false;
	bool run = true;

	renderer->draw_game_with_batch(false);
	renderer->draw_message_box(_T("开启推演模式"));
	Sleep(1000);

	while (true)
	{
		renderer->draw_game_with_batch(false);

		while (end || !player->go(who)) {
			if (player->isChange())
				renderer->draw_game_with_batch(false);
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				if (GetAsyncKeyState('Z') & 0x8000 && game_record.size() > 6) {
					deduce_reget();
					who = who == 'r' ? 'b' : 'r';
					end = false;
					renderer->draw_game_with_batch(false);
					Sleep(100);
				}
			}
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				run = false;
				break;
			}
		}

		if (!run)
		{
			break;
		}

		new_record();

		renderer->draw_game_with_batch(false);

		if (tool::isWin(who)) {
			renderer->draw_game_final_sign(who);
			Sleep(1000);
			end = true;
			who = who == 'r' ? 'b' : 'r';
			continue;
		}

		if (tool::isCheck(who)) {
			renderer->draw_game_with_batch(true);
			Sleep(1000);
		}
		who = who == 'r' ? 'b' : 'r';
	}
	
	chess_board = chess_board_copy;
	select_piece->set(select_piece_copy);
	last_move_begin_coordinate->set(last_move_begin_coordinate_copy);
	last_move_end_coordinate->set(last_move_end_coordinate_copy);
	destination_set = destination_set_copy;
	game_record = game_record_copy;

	renderer->draw_message_box(_T("结束推演模式"));
	Sleep(1000);
}
