#include "Game.h"

void Game::Init()
{
	player = new Player();
	ai = new Ai();
	renderer = new Renderer();

	chess_board = tool::readFileGetChessBoard(chessboard_txt_path);
	tool::Cma = new cma::ChessMovesAnalyze<std::string>(chess_board);
	tool::Cma->bind_result_set(destination_set);
	select_piece = new coordinate(-1, -1);
	last_move_begin_coordinate = new coordinate(-1, -1);
	last_move_end_coordinate = new coordinate(-1, -1);

	player->Bind(&chess_board, select_piece, last_move_begin_coordinate, 
		last_move_end_coordinate, &destination_set);
	ai->Bind(&chess_board, last_move_begin_coordinate, last_move_end_coordinate, 
		&keepCheckTimes, &keepCheckRecord);
	renderer->Bind(&chess_board, select_piece, last_move_begin_coordinate, 
		last_move_end_coordinate, &destination_set);
	reSet();
}

Game::Game()
{
	LOG("GAME INIT!");

	Init();

	while (true) {
		LOG("GAME START!");

		renderer->draw_start_sign();

		while (true)
		{
			renderer->draw(false);

			bool tie = false;

			while (!player->go()) {
				if (player->isChange())
					renderer->draw(false);
				if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
					if (GetAsyncKeyState(0x54) & 0x8000) {
						tie = true;
						break;
					}
				}
			}

			if (tie)
			{
				winner = 'n';
				win_way = _T("和棋");
				destination_set.clear();
				select_piece->set(-1, -1);
				renderer->draw(false);
				renderer->draw_final_sign(winner);
				Sleep(1000);
				break;
			}

			new_record();

			renderer->draw(false);

			if (tool::isWin('r')) {
				winner = 'r';
				win_way = tool::isCheck('r') ? _T("红方绝杀") : _T("黑方困毙");
				renderer->draw_final_sign(winner);
				Sleep(1000);
				break;
			}

			if (tool::isCheck('r')) {
				renderer->draw(true);
				Sleep(1000);
			}

			renderer->draw(false);

			bool respons = ai->go();

			new_record();

			if (respons)
			{
				winner = 'r';
				win_way = _T("黑方掀棋盘");
				renderer->draw_final_sign(winner);
				Sleep(1000);
				break;
			}

			renderer->draw(false);

			if (tool::isWin('b')) {
				winner = 'b';
				win_way = tool::isCheck('b') ? _T("黑方绝杀") : _T("红方困毙");
				renderer->draw_final_sign(winner);
				Sleep(1000);
				break;
			}

			if (tool::isCheck('b')) {
				keepCheckTimes++;
				keepCheckRecord.insert(tool::chessboardToFanStr(chess_board));
				renderer->draw(true);
				Sleep(1000);
			}
			else
			{
				keepCheckTimes = 0;
				keepCheckRecord.clear();
			}
		}

		save_record();

		LOG("GAME OVER!");

		if (reNew(winner)) break;

		reSet();
	}
	LOG("PROGRAM EXIT!");
}

Game::~Game()
{
	delete player;
	delete ai;
	delete renderer;
	delete select_piece;
	delete last_move_begin_coordinate;
	delete last_move_end_coordinate;
	delete tool::Cma;
	LOG("GAME END!");
}

void Game::reSet()
{
	chess_board = tool::readFileGetChessBoard(chessboard_txt_path);
	select_piece->set(-1, -1);
	last_move_begin_coordinate->set(-1, -1);
	last_move_end_coordinate->set(-1, -1);
	destination_set.clear();
	winner = ' ';
	win_way = _T("");
	game_record = "----";
	keepCheckTimes = 0;
	keepCheckRecord.clear();
}

void Game::new_record()
{
	game_record += last_move_begin_coordinate->to_string() + 
		last_move_end_coordinate->to_string();
}

void Game::save_record()
{
	tool::writeFileSaveRecord(record_txt_path, game_record);
}

bool Game::reNew(char winner)
{
	renderer->draw_renew(winner, win_way);
	int mx, my;
	bool leftButton;
	button renew(130, 500, 365, 570);
	button checkEnd(375, 500, 610, 570);
	button review(130, 580, 365, 650);
	button exit(375, 580, 610, 650);
	while (true)
	{
		bool flip = false;
		tool::getMouseMessage(mx, my, leftButton);
		if (renew.is_click(mx, my, leftButton))
		{
			return false;
		} 
		else if (checkEnd.is_click(mx, my, leftButton))
		{
			this->view();
			flip = true;
		}
		else if (review.is_click(mx, my, leftButton))
		{
			this->review();
			flip = true;
		}
		else if (exit.is_click(mx, my, leftButton))
		{
			return true;
		}

		if (flip)
			renderer->draw_renew(winner, win_way);
	}
	return false;
}

void Game::view()
{
	renderer->draw(false);
	while (true)
	{
		bool anyKeyPressed = false;
		int PressedKey;

		for (int i = 0x41; i <= 0x5A; ++i) { // A-Z
			if (GetAsyncKeyState(i) & 0x8000) {
				anyKeyPressed = true;
				PressedKey = i;
				break;
			}
		}
		if (anyKeyPressed) {
			while (GetAsyncKeyState(PressedKey) & 0x8000);
			break;
		}
	}
}

void Game::review()
{
	game_record = tool::readFileGetRecord(record_txt_path);
	chess_board = tool::readFileGetChessBoard("./chess.txt");
	last_move_begin_coordinate->set(-1, -1);
	last_move_end_coordinate->set(-1, -1);
	renderer->draw(false);

	int pointer = 0;
	std::vector<std::string> records;
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
				pointer += 4;
				right = true;
				flip = true;
			}
		}

		if (flip)
		{
			review_reSetInfo(pointer, records, right);
			renderer->draw_record(pointer, game_record.size());
			Sleep(100);
		}
	}
}

void Game::review_reSetInfo(int& ptr, std::vector<std::string>& records, bool right)
{
	if (!right && (ptr == 0 || ptr >= game_record.size()))
	{
		ptr -= 4;
		return; 
	} 
	else if (right && (ptr == 0 || ptr >= game_record.size()))
	{
		return;
	}

	coordinate begin(game_record.substr(ptr, 2));
	coordinate end(game_record.substr(ptr + 2, 2));

	if (right)
	{
		records.push_back(chess_board[end.y][end.x]);
		chess_board[end.y][end.x] = chess_board[begin.y][begin.x];
		chess_board[begin.y][begin.x] = "sp";
	}
	else
	{
		chess_board[begin.y][begin.x] = chess_board[end.y][end.x];
		chess_board[end.y][end.x] = records.back();
		records.pop_back();
		ptr -= 4;
		if (ptr == 0)
		{
			begin.set(-1, -1);
			end.set(-1, -1);
		}
		else 
		{
			begin.set(game_record.substr(ptr, 2));
			end.set(game_record.substr(ptr + 2, 2));
		}
	}

	last_move_begin_coordinate->set(begin);
	last_move_end_coordinate->set(end);
}
