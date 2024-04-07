#include "Interface.h"

Interface::Interface()
{
	FileHelper::getAllRecordsInFoder(recent_record_list_path, recent_record_list);
	FileHelper::getAllRecordsInFoder(my_record_list_path, my_record_list);
	FileHelper::getAllRecordsInFoder(load_record_list_path, load_record_list);
	FileHelper::getLinkedRecord(recent_record_list, my_record_list, linked_record);

	FileHelper::readFileGetChessBoard(PublicResource::current_chessBoard);
	FileHelper::readFileGetOriginalChessBoard(PublicResource::current_original_chessBoard);
	PublicResource::current_amount = FileHelper::readFileGetAIAmount();
	PublicResource::current_go_first = FileHelper::readFileGetGoFirst();
	PublicResource::current_automatically_add = FileHelper::readFileGetAutomaticallyAdd();
	PublicResource::current_recent_id = FileHelper::readFileGetId(recent_record_list_path);
	PublicResource::current_my_id = FileHelper::readFileGetId(my_record_list_path);
	PublicResource::current_load_id = FileHelper::readFileGetId(load_record_list_path);

	renderer = new Renderer();
	game = new Game(renderer);

	main_interface();
}

Interface::~Interface()
{
	delete renderer;
	delete game;
}

void Interface::main_interface()
{
	renderer->draw_main_interface();
	Button play(15, 650, 347, 70);
	Button settings(377, 650, 347, 70);
	Button record_control(15, 735, 347, 70);
	Button import_record(377, 735, 347, 70);
	while (true)
	{
		bool flip = false;
		MouseControl::update_mouse_message();
		if (play.click_listen())
		{
			delete game;
			game = new Game(renderer);
			game->Bind(&recent_record_list, &my_record_list, &load_record_list, &linked_record);
			game->start();
			flip = true;
		}
		else if (settings.click_listen())
		{
			settings_interface();
			flip = true;
		}
		else if (record_control.click_listen())
		{
			manage_interface();
			flip = true;
		}
		else if (import_record.click_listen())
		{
			load_interface();
			flip = true;
		}
		if (flip)
			renderer->draw_main_interface();
	}
}

void Interface::manage_interface()
{
	size_t curr_carte_id = 0;
	size_t curr_func_id = 0;
	size_t page = 0;
	size_t total_page = 0;
	std::string curr_file_path;
	std::vector<Record>* records_ptr = &recent_record_list;

	radio_button recent_records_rb(0, 0, 247, 100, 0, &curr_carte_id);
	radio_button my_records_rb(248, 1, 247, 100, 1, &curr_carte_id);
	radio_button load_records_rb(495, 1, 247, 100, 2, &curr_carte_id);

	std::vector<std::vector<radio_button>> radio_buttons =
	{
		{
			{20, 120, 220, 70, 0, &curr_func_id},
			{260, 120, 220, 70, 1, &curr_func_id},
			{500, 120, 220, 70, 2, &curr_func_id}
		},
		{
			{20, 120, 160, 70, 0, &curr_func_id},
			{200, 120, 160, 70, 1, &curr_func_id},
			{380, 120, 160, 70, 2, &curr_func_id},
			{560, 120, 160, 70, 3, &curr_func_id}
		},
		{
			{20, 120, 160, 70, 0, &curr_func_id},
			{200, 120, 160, 70, 1, &curr_func_id},
			{380, 120, 160, 70, 2, &curr_func_id},
			{560, 120, 160, 70, 3, &curr_func_id}
		}
	};

	std::vector<Button> record_buttons;

	for (int y = 0; y < record_page_limit; y++)
	{
		record_buttons.push_back({ 575, 210 + y * 70, 145, 50 });
	}

	Button prev_page(130, 763, 110, 45);
	Button next_page(500, 763, 110, 45);
	Button front_page(40, 763, 80, 45);
	Button end_page(620, 763, 80, 45);

	bool change_carte = true;
	bool change_func = true;
	bool change_page = true;
	bool flip = true;
	while (true)
	{
		MouseControl::update_mouse_message();

		change_carte |= recent_records_rb.click_listen();
		change_carte |= my_records_rb.click_listen();
		change_carte |= load_records_rb.click_listen();

		for (int i = 0; i < radio_buttons[curr_carte_id].size(); i++)
		{
			change_func |= radio_buttons[curr_carte_id][i].click_listen();
		}

		std::vector<Record>& records = *records_ptr;

		int limit = min((int)records.size() - (int)page * record_page_limit, record_page_limit);
		for (int i = 0; i < limit; i++)
		{
			if (record_buttons[i].click_listen())
			{
				std::wstring curr_func = PublicResource::manage_interface_functions[curr_carte_id][curr_func_id];
				Record& curr_record = records[page * record_page_limit + i];
				flip = true;
				if (curr_func == _T("查看对局"))
				{
					manage_interface_check_record(curr_record);
				}
				else if (curr_func == _T("加入我的对局"))
				{
					manage_interface_join_record(curr_record);
				}
				else if (curr_func == _T("分享对局") || curr_func == _T("转发对局"))
				{
					manage_interface_share_record(curr_record);
				}
				else if (curr_func == _T("删除对局"))
				{
					manage_interface_delete_record(curr_record, curr_file_path, page* record_page_limit + i, records);
				}
				else if (curr_func == _T("更改标签"))
				{
					manage_interface_edit_tickets(curr_record);
				}
				else if (curr_func == _T("查看标签"))
				{
					manage_interface_check_tickets(curr_record);
				}
				break;
			}
		}

		if (records.size())
		{
			if (prev_page.click_listen())
			{
				if (page)
				{
					page--;
					change_page = true;
				}
				else
				{
					renderer->draw_message_box(_T("已经是第一页"));
					Sleep(1000);
					flip = true;
				}
			}
			else if (next_page.click_listen())
			{
				if (page < total_page)
				{
					page++;
					change_page = true;
				}
				else
				{
					renderer->draw_message_box(_T("已经是最后一页"));
					Sleep(1000);
					flip = true;
				}
			}
			else if (front_page.click_listen())
			{
				if (page != 0)
				{
					page = 0;
					change_page = true;
				}
				else
				{
					renderer->draw_message_box(_T("已经是第一页"));
					Sleep(1000);
					flip = true;
				}
			}
			else if (end_page.click_listen())
			{
				if (page < total_page)
				{
					page = tool::getTotalPages((int)records.size());
					change_page = true;
				}
				else
				{
					renderer->draw_message_box(_T("已经是最后一页"));
					Sleep(1000);
					flip = true;
				}
			}
		}

		if (change_carte || change_func || change_page || flip)
		{
			if (change_carte)
			{
				curr_file_path = curr_carte_id == 0 ? recent_record_list_path : (curr_carte_id == 1 ? my_record_list_path : load_record_list_path);
				curr_func_id = 0;
				records_ptr = curr_carte_id == 0 ? &recent_record_list : (curr_carte_id == 1 ? &my_record_list : &load_record_list);
				page = 0;
				total_page = tool::getTotalPages((int)records_ptr->size());
			}
			renderer->draw_manage_interface_carte(curr_carte_id, curr_func_id, 
				PublicResource::manage_interface_functions, PublicResource::manage_interface_colors);
			renderer->draw_manage_interface_records(PublicResource::manage_interface_functions[curr_carte_id][curr_func_id], *records_ptr, linked_record, 
				page, PublicResource::manage_interface_func_to_label.at(curr_carte_id * 10 + curr_func_id), PublicResource::manage_interface_colors[curr_carte_id][curr_func_id]);
			change_func = false;
			change_carte = false;
			change_page = false;
			flip = false;
		}

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			break;
		}
	}
}

void Interface::manage_interface_check_record(Record& record)
{
	FileHelper::copyFile(record.path, curr_review_code_txt_path);
	game->review();
	while (GetAsyncKeyState(VK_ESCAPE) & 0x8000);
}

void Interface::manage_interface_join_record(Record& record)
{
	// Check the record was or was not linked
	int linked = false; // linked sigh
	for (int i = 0; i < linked_record.size(); i++)
	{
		if (linked_record[i].first == record)
		{
			linked = true; // update sigh
			break;
		}
	}

	if (!linked)
	{
		// Copy the file into my record list
		std::string my_controlfile = my_record_list_path + "/control.txt";
		std::string my_filename = std::to_string(PublicResource::current_my_id + 1) + ".txt";
		std::string my_filepath = my_record_list_path + "/" + my_filename;
		FileHelper::copyFile(record.path, my_filepath);

		// Update the max number of my record list
		PublicResource::current_my_id++;

		// Add the filename into the beginning control file 
		FileHelper::insertContentAtBeginning(my_controlfile, my_filename);

		// Copy the selected record into the front of the my record list 
		my_record_list.insert(my_record_list.begin(), record);
		my_record_list.front().path = my_filepath;
		 
		// Add a link in to join.txt ( format : recent-record.txt/my-record.txt )
		std::string joinfile = recent_record_list_path + "/join.txt";
		std::string resent_filename = record.path.substr(recent_record_list_path.size() + 1);
		std::string link_code = resent_filename + "/" + my_filename;
		std::ofstream join_ofs(joinfile, std::ios_base::app); // append mode
		join_ofs << link_code << std::endl;
		join_ofs.close();

		// Update linked record 
		linked_record.push_back({ record , my_record_list.front()});

		// Print a Message about linked successfully
		renderer->draw_message_box(_T("加入成功！"));
		Sleep(1000);
	}
	else
	{
		// If the record was linked, print a message
		renderer->draw_message_box(_T("该对局已经加入！"));
		Sleep(1000);
	}
}

void Interface::manage_interface_share_record(Record& record)
{
	std::ifstream ifs(record.path);
	std::string line;
	std::getline(ifs, line);
	ifs.close();
	tool::copy(line);
	renderer->draw_message_box(_T("已复制对局码！"));
	Sleep(1000);
}

void Interface::manage_interface_delete_record(Record& record, std::string foder, size_t id, std::vector<Record>& records)
{
	// Save the file name and path of record
	std::string filepath = record.path;
	std::string filename = filepath.substr(foder.size() + 1);

	// Delete the record in records
	records.erase(records.begin() + id);

	// Delete the file
	fs::remove(filepath);

	// Delete the filename in control.txt
	std::string controlfile = foder + "/control.txt";
	FileHelper::removeLineInFile(controlfile, filename);

	if (foder == my_record_list_path)
	{
		// Check was it linked
		bool is_linked = false; // a sigh express it's linked 
		std::string target; // The target line is if the last game was linked, the line in the file join.txt that we need to delete 
		for (int i = 0; i < linked_record.size(); i++)
		{
			if (linked_record[i].second.path == filepath)
			{
				// If the record was linked, unlink it
				std::string first = linked_record[i].first.path.substr(recent_record_list_path.size() + 1);
				linked_record.erase(linked_record.begin() + i);
				target = first + "/" + filename; // update the target
				is_linked = true; // update the sigh
				break;
			}
		}

		if (is_linked)
		{
			// If the game was linked, delete the target line in the file join.txt
			FileHelper::removeLineInFile(recent_record_list_path + "/join.txt", target);
		}
	}

	renderer->draw_message_box(_T("删除成功！"));
	Sleep(1000);
}

void Interface::manage_interface_edit_tickets(Record& record)
{
	Button exit(127, 247, 50, 50);

	std::vector<Button> buttons =
	{
		{160, 355, 90, 90},
		{270, 355, 90, 90},
		{380, 355, 90, 90},
		{490, 355, 90, 90},
	};

	bool flip = true;
	bool change = false;
	while (true)
	{
		MouseControl::update_mouse_message();
		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i].click_listen())
			{
				record.tickets[i] = !record.tickets[i];
				flip = true;
				change = true;
				break;
			}
		}
		if (exit.click_listen())
		{
			break;
		}
		if (flip)
		{
			// Draw the edit-window 
			renderer->draw_manage_interface_tickets(record.tickets);
			flip = false;
		}
	}

	if (change)
	{
		// Update the change in file 
		// 1. read the first line of the file
		std::ifstream ifs(record.path);
		std::string line;
		std::getline(ifs, line);
		ifs.close();

		// 2. position the ticket-info in the line
		size_t ptr = 0;
		size_t cnt = 0;
		while (cnt < 4)
		{
			cnt += line[ptr] == '/';
			ptr++;
		}

		// 3. trun the tickets to string for replace
		std::string replacement = "";
		for (int i = 0; i < record.tickets.size(); i++)
		{
			replacement += std::to_string(record.tickets[i]);
		}
		
		// 4. replace the original ticket-info
		line.replace(ptr, replacement.size(), replacement);

		// 5. write the line back to the file
		std::ofstream ofs(record.path);
		ofs << line;
		ofs.close();

	}
}

void Interface::manage_interface_check_tickets(Record& record)
{
	Button exit(127, 247, 50, 50);
	bool have_ticket = std::find(record.tickets.begin(), record.tickets.end(), true) != record.tickets.end();;
	if (have_ticket)
	{
		renderer->draw_manage_interface_tickets(record.tickets);
		do
		{
			MouseControl::update_mouse_message();
		} while (!exit.click_listen());
	}
	else
	{
		renderer->draw_message_box(_T("该对局没有标签！"));
		Sleep(1000);
	}
}

void Interface::settings_interface()
{
	size_t amount = PublicResource::current_amount;
	size_t go_first = PublicResource::current_go_first;
	bool auto_add = PublicResource::current_automatically_add;
	CHESS_BOARD chess_board = PublicResource::current_chessBoard;
	std::vector<size_t> piece_num;
	settings_interface_init_piece_num(piece_num);
	std::vector<coordinate> selected = { {-1,-1} , {-1,-1} };
	
	slider amount_slider(195, 24, 24, 220, 420, &amount, &PublicResource::settings_interface_slider_values);
	radio_button red_radiob(205, 255, 80, 40, 1, &go_first);
	radio_button black_radiob(295, 255, 80, 40, 0, &go_first);
	Button auto_b(610, 250, 50, 50);
	std::vector<Button> piece_buttons;
	Button resetb(620, 360, 100, 100);
	Button clearb(620, 480, 100, 100);
	Button cansolb(620, 600, 100, 100);
	Button saveb(470, 750, 120, 50);
	Button exit(600, 750, 120, 50);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			int curr_x = 473 + i * 70;
			int curr_y = 333 + j * 70;
			piece_buttons.push_back({ curr_x , curr_y, 54, 54 });
		}
	}

	bool flip = false;
	bool change = false;
	renderer->Bind(&chess_board, nullptr, nullptr, nullptr, nullptr);

	renderer->draw_settings_interface(amount, go_first, auto_add, piece_num, selected, change);

	while (true)
	{
		MouseControl::update_mouse_message();

		flip |= red_radiob.click_listen();
		flip |= black_radiob.click_listen();

		if (auto_b.click_listen())
		{
			auto_add = !auto_add;
			flip = true;
		}

		flip |= amount_slider.listen();
		change |= flip;

		if (!flip)
		{
			int mx, my;
			bool lbutton = false, rbutton = false;

			tool::getMouseMessage(mx, my, lbutton);
			rbutton = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
			if (mx >= 23 && mx <= 434 && my >= 323 && my <= 782)
			{
				mx -= 50;
				my -= 350;
				int gridx = max(0, (int)std::round((float)mx / 48.0f));
				int gridy = max(0, (int)std::round((float)my / 48.0f));
				std::string item = chess_board[gridy][gridx];
				std::string select_item = "";
				if (!selected[0].isEmpty())
				{
					if (selected[0].y >= 0)
					{					
						select_item = chess_board[selected[0].y][selected[0].x];
					}
					else
					{
						select_item = PublicResource::piece_type[selected[0].x + (selected[0].x > 5)];
					}
				}
				std::string target_item = selected[1].isEmpty() ? "" : chess_board[selected[1].y][selected[1].x];
				if (lbutton)
				{
					if (item != "sp" && (selected[0].x != gridx || selected[0].y != gridy))
					{
						selected[0].set(gridx, gridy);
						selected[1].clear();
						flip = true;
					}
				}
				else if (rbutton)
				{
					if (!selected[1].isEmpty())
					{
						if (target_item[1] == 'l')
						{
							renderer->draw_message_box(_T("无法删除“将”"));
							Sleep(1000);
						}
						else
						{
							if (target_item != "sp")
							{
								for (int x = 0; x < PublicResource::piece_type.size(); x++)
								{
									if (PublicResource::piece_type[x] == target_item)
									{
										piece_num[x - (x > 6)]++;
										break;
									}
								}
							}
							chess_board[selected[1].y][selected[1].x] = select_item;
							if (selected[0].y >= 0)
							{
								chess_board[selected[0].y][selected[0].x] = "sp";
							}
							else
							{
								piece_num[selected[0].x]--;
							}
							selected[0].clear();
							selected[1].clear();
							change = true;
						}
						flip = true;
					}
				}
				if (!selected[0].isEmpty() && (selected[1].x != gridx || selected[1].y != gridy) && 
					(selected[0].y >= 0 || piece_num[selected[0].x]))
				{
					int reversedx = select_item[0] == 'r' ? GRID_WIDTH - gridx - 1 : gridx;
					int reversedy = select_item[0] == 'r' ? GRID_HEIGHT - gridy - 1 : gridy;
					if (PublicResource::settings_interface_piece_legal_position.find(select_item[1]) == PublicResource::settings_interface_piece_legal_position.end() || 
						PublicResource::settings_interface_piece_legal_position.at(select_item[1])[reversedy][reversedx])
					{
						selected[1].x = gridx;
						selected[1].y = gridy;
						flip = true;
					}
					else
					{
						selected[1].clear();
						flip = true;
					}
				}
			}
			else if (!selected[1].isEmpty())
			{
				selected[1].clear();
				flip = true;
			}
		}

		if (!flip)
		{
			for (int i = 0; i < piece_buttons.size(); i++)
			{
				if (piece_buttons[i].click_listen())
				{
					selected[0].set(i, -1);
					flip = true;
				}
			}
		}

		if (resetb.click_listen())
		{
			chess_board = PublicResource::current_original_chessBoard;
			selected[0].clear();
			selected[1].clear();
			for (int i = 0; i < piece_num.size(); i++)
			{
				piece_num[i] = 0;
			}
			flip = true;
			change = true;
		}

		if (clearb.click_listen())
		{
			for (int i = 0; i < GRID_HEIGHT; i++)
			{
				for (int j = 0; j < GRID_WIDTH; j++)
				{
					chess_board[i][j] = "sp";
				}
			}
			chess_board[0][4] = "bl";
			chess_board[GRID_HEIGHT - 1][4] = "rl";
			for (int i = 0; i < piece_num.size(); i++)
			{
				if (i % 6 != 0)
				{
					piece_num[i] = 2;
				}
				else
				{
					piece_num[i] = 5;
				}
			}
			flip = true;
			change = true;
		}

		if (cansolb.click_listen())
		{
			chess_board = PublicResource::current_chessBoard;
			settings_interface_init_piece_num(piece_num);
			renderer->draw_message_box(_T("撤销成功！"));
			Sleep(1000);
			flip = true;
		}

		if (saveb.click_listen())
		{
			if (change)
			{
				PublicResource::Cma = new cma::ChessMovesAnalyze(chess_board);
				CMA_RESULT_SET result;
				PublicResource::Cma->bind_result_set(result);
				if (tool::isCheck('r') || tool::isCheck('b'))
				{
					renderer->draw_message_box(_T("初始盘面不能将军！"));
					Sleep(1000);
				}
				else if (tool::isWin(go_first ? 'b' : 'r'))
				{
					renderer->draw_message_box(_T("初始盘面不能困毙！"));
					Sleep(1000);
				}
				else if (tool::is_tie(chess_board))
				{
					renderer->draw_message_box(_T("初始盘面不能和棋！"));
					Sleep(1000);
				}
				else
				{
					// save data
					// 1. save amount
					std::ofstream amount_ofs(amount_txt_path);
					amount_ofs << amount << std::endl;
					amount_ofs.close();

					// 2. save go-first
					std::ofstream first_ofs(go_first_txt_path);
					first_ofs << (go_first ? "player" : "ai") << std::endl;
					first_ofs.close();

					// 3. save auto-add-into-mylist
					std::ofstream auto_ofs(automatically_add_txt_path);
					auto_ofs << auto_add << std::endl;
					auto_ofs.close();

					// 4. save chessboard
					std::string input = tool::chessboardToFanStr(chess_board);
					std::ofstream chessboard_ofs(chessboard_txt_path);
					chessboard_ofs << input << std::endl;
					chessboard_ofs.close();

					// update info
					// 1. update amount
					PublicResource::current_amount = amount;

					// 2. update go-first
					PublicResource::current_go_first = go_first;

					// 3. update auto-add-into-mylist
					PublicResource::current_automatically_add = auto_add;

					// 4. update chessboard
					PublicResource::current_chessBoard = chess_board;

					change = false;
					renderer->draw_message_box(_T("保存成功！"));
					Sleep(1000);
				}
				delete PublicResource::Cma;
				PublicResource::Cma = nullptr;
			}
			else
			{
				renderer->draw_message_box(_T("未检测到更改！"));
				Sleep(1000);
			}
			flip = true;
		}

		if (exit.click_listen())
		{
			if (!change || settings_interface_reminder())
			{
				break;
			}
			flip = true;
		}

		if (flip)
		{
			bool record = PublicResource::current_go_first;
			PublicResource::current_go_first = go_first;
			renderer->draw_settings_interface(amount, go_first, auto_add, piece_num, selected, change);
			PublicResource::current_go_first = record;
			flip = false;
		}
	}
}

void Interface::settings_interface_init_piece_num(std::vector<size_t>& piece_num)
{
	piece_num.clear();
	for (int i = 0; i < 12; i++)
	{
		if (i % 6 != 0)
		{
			piece_num.push_back(2);
		}
		else
		{
			piece_num.push_back(5);
		}
	}
	CHESS_BOARD& chess_board = PublicResource::current_chessBoard;
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (chess_board[i][j][1] != 'l')
			{
				for (int x = 0; x < PublicResource::piece_type.size(); x++)
				{
					if (PublicResource::piece_type[x] == chess_board[i][j])
					{
						piece_num[x - (x > 6)]--;
						break;
					}
				}
			}
		}
	}
}

bool Interface::settings_interface_reminder()
{
	Button exit(140, 510, 220, 50);
	Button cansol(380, 510, 220, 50);
	renderer->draw_settings_interface_reminder();
	while (true)
	{
		MouseControl::update_mouse_message();

		if (exit.click_listen())
		{
			return true;
		}

		if (cansol.click_listen())
		{
			return false;
		}
	}
	return false;
}

void Interface::load_interface()
{
	size_t step = 0;
	Button button1(80, 720, 250, 80);
	Button button2(410, 720, 250, 80);
	bool running = true;

	while (running)
	{
		switch (step)
		{
		case 0:
			// first step - copy review code
			renderer->draw_load_interface_step1();
			while (true)
			{
				MouseControl::update_mouse_message();

				if (button1.click_listen())
				{
					return;
				}
				else if (button2.click_listen())
				{
					// go to next step
					step = 1;
					break;
				}
			}
			break;
		case 1:
			// The second step - paste review code
			Button text_box(63, 463, 600, 25);
			bool select = false;
			std::string info = "";
			bool flip = true;
			while (true)
			{
				MouseControl::update_mouse_message();

				if (button1.click_listen())
				{
					// go to prev step
					step = 0;
					break;
				}
				else if (button2.click_listen())
				{
					if (!info.size())
					{
						renderer->draw_message_box(_T("请先粘贴复盘码！"));
						Sleep(1000);
						flip = true;
					}
					else
					{
						std::string filename = std::to_string(PublicResource::current_load_id + 1) + ".txt";
						std::string filepath = load_record_list_path + "/" + filename;
						std::string code = std::string(info.begin(), info.end());
						try
						{
							Record result = tool::code_to_record(code, filepath);
							PublicResource::current_load_id++;

							// Create a new file in load record list
							std::ofstream ofs(filepath);
							ofs << code << std::endl;
							ofs.close();

							// Add the file name into the control.txt
							std::string load_controlfile = load_record_list_path + "/control.txt";
							FileHelper::insertContentAtBeginning(load_controlfile, filename);

							// Update the load_record_list
							load_record_list.insert(load_record_list.begin(), result);

							// Output a message about load successfully
							renderer->draw_message_box(_T("导入成功！"));
							Sleep(1000);
							running = false;
							break;
						}
						catch (const std::exception&)
						{
							renderer->draw_message_box(_T("非法复盘码！"));
							Sleep(1000);
							flip = true;
						}
					}
				}
				else if (text_box.click_listen() && !select)
				{
					select = true;
					flip = true;
				}

				if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('V') & 0x8000))
				{
					if (select)
					{
						info = tool::paste();
						flip = true;
					}
					else
					{
						renderer->draw_message_box(_T("请先选中文本框！"));
						Sleep(1000);
						flip = true;
					}
				}

				if (flip)
				{
					renderer->draw_load_interface_step2(select, std::wstring(info.begin(), info.end()));
					flip = false;
				}
			}
			break;
		}
	}
}

