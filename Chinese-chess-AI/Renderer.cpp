#include "Renderer.h"
#include "tool.h"
#include "PublicResource.h"

#define TCG(xy) (int)(edge + spacing * chessboard_size * (xy))

Renderer::Renderer() 
	: edge(50), spacing((WINDOW_HEIGHT - edge * 2) / (GRID_HEIGHT - 1)),
	  chess_board(nullptr), select_piece(nullptr),
	  last_move_begin_coordinate(nullptr),
	  last_move_end_coordinate(nullptr),
	  destination_set(nullptr), chessboard_size(1.0f), 
	  chessboard_x(0), chessboard_y(0)
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, _T("象棋AI对战"));
	HICON hIcon = (HICON)LoadImage(NULL, chess_ico_path, 
		IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	setbkcolor(BG_COLOR);
	setbkmode(TRANSPARENT);
	load_imgs();
	
}

Renderer::~Renderer()
{
	closegraph();
	
}

void Renderer::Bind(CHESS_BOARD* chess_board,
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

void Renderer::draw_main_interface()
{
	draw_start();
	clear();
	std::wstring text = _T("象棋Ai对战");
	settextcolor(BLACK);
	settextstyle(70, 0, _T("宋体"));
	int textWidth = textwidth(text.c_str());
	int x = (WINDOW_WIDTH - textWidth) / 2;
	int y = 59;
	outtextxy(x, y, text.c_str());
	putimage(50, 150, &main_interface);

	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 2);
	int edge = 15;
	int button_width = (WINDOW_WIDTH - 3 * edge) / 2;
	int button_height = 70;
	int text_size = 40;
	int start_y = WINDOW_HEIGHT - (button_height + edge) * 2;
	std::vector<std::wstring> info =
	{
		_T("开始游戏"), _T("设置"), _T("对局管理"), _T("上传对局")
	};
	int ptr = 0;
	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 2; x++) {
			draw_texture_in_box_center(edge + x * (edge + button_width), 
				start_y + y * (edge + button_height), 
				edge + x * (edge + button_width) + button_width,
				start_y + y * (edge + button_height) + button_height,
				text_size, info[ptr].c_str(), _T("宋体"));
			ptr++;
		}
	}
	draw_end();
}

void Renderer::draw_manage_interface_carte(size_t carte_id, size_t func_id, const std::vector<std::vector<std::wstring>>& functions, const std::vector<std::vector<COLORREF>>& colors)
{
	draw_start();
	clear();

	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);

	std::vector<std::wstring> crate = {
		_T("最近对局"), _T("我的对局"), _T("导入对局")
	};

	int y = 100;
	for (int x = 0; x < crate.size(); x++)
	{
		draw_texture_in_box_center(1 + x * (WINDOW_WIDTH / 3), 1, 
			1 + (x + 1) * (WINDOW_WIDTH / 3), y, 30, 
			crate[x].c_str(), _T("黑体"), BLACK);
	}

	setlinecolor(BLUE);
	setlinestyle(PS_SOLID, 5);

	int edge = 15;
	rectangle((int)carte_id * (WINDOW_WIDTH / 3) + edge, edge,
		((int)carte_id + 1) * (WINDOW_WIDTH / 3) - edge, y - edge);

	y = 120;
	edge = 20;
	int text_size = 30;
	int list_size = (int)functions[carte_id].size();
	int button_width = (WINDOW_WIDTH - edge * (list_size + 1)) / list_size;
	int button_height = 70;
	for (int x = 0; x < list_size; x++)
	{
		setfillcolor(colors[carte_id][x]);
		draw_texture_in_box_center(edge + x * (button_width + edge), y,
			(x + 1) * (button_width + edge), y + button_height,
			30, functions[carte_id][x].c_str(), _T("黑体"), BLACK, true);
	}
	setlinecolor(BLUE);
	setlinestyle(PS_SOLID, 5);
	rectangle(edge + (int)func_id * (button_width + edge) + 10, y + 10,
		((int)func_id + 1) * (button_width + edge) - 10, y + button_height - 10);
}

void Renderer::draw_manage_interface_records(std::wstring func_name, std::vector<Record>& records, std::vector<std::pair<Record, Record>> linked_record, size_t page, std::wstring button_lable, COLORREF button_color)
{
	int begin = (int)page * record_page_limit;
	int end = begin + min((int)records.size() - begin, record_page_limit);

	if (!end)
	{
		settextcolor(BLACK);
		settextstyle(50, 0, _T("黑体"));
		std::wstring text = _T("这里空空如也！");
		int x = (WINDOW_WIDTH - textwidth(text.c_str())) / 2;
		int y = (WINDOW_WIDTH + 170) / 2;
		outtextxy(x, y, text.c_str());
	}
	else
	{
		for (int y = 0; begin < end; begin++, y++)
		{
			if (func_name == _T("加入我的对局"))
			{
				button_lable = _T("加入");
				for (int i = 0; i < linked_record.size(); i++)
				{
					if (linked_record[i].first == records[begin])
					{
						button_lable = _T("已加入");
						break;
					}
				}
			}
			draw_manage_interface_record(20, 210 + y * 70, button_lable, button_color, records[begin]);
		}

		int total_page = tool::getTotalPages((int)records.size());
		std::wstring text = _T("第 ") + std::to_wstring(page + 1) + _T(" / ") +
			std::to_wstring(total_page + 1) + _T(" 页");
		int text_size = 40;
		settextcolor(BLACK);
		settextstyle(text_size, 0, _T("黑体"));
		int text_width = textwidth(text.c_str());
		int x = (WINDOW_WIDTH - text_width) / 2;
		outtextxy(x, 767, text.c_str());

		int y = 763;
		int edge = 20;
		int button_width = 110;
		int button_height = 45;
		setlinestyle(PS_SOLID, 3);
		draw_texture_in_box_center(x - button_width - edge, y, x - edge, y + button_height,
			30, _T("上一页"), _T("黑体"));
		draw_texture_in_box_center(x + text_width + edge, y, x + text_width + edge + button_width, y + button_height,
			30, _T("下一页"), _T("黑体"));

		button_width = 80;
		draw_texture_in_box_center(edge * 2, y, edge * 2 + button_width, y + button_height,
			30, _T("首页"), _T("黑体"));
		draw_texture_in_box_center(WINDOW_WIDTH - edge * 2, y, WINDOW_WIDTH - edge * 2 - button_width, y + button_height,
			30, _T("尾页"), _T("黑体"));
	}
	draw_end();
}

void Renderer::draw_manage_interface_record(int x, int y, std::wstring func_name, COLORREF button_color, Record& record)
{
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 5);
	int width = WINDOW_WIDTH - 40;
	int height = 50;
	rectangle(x, y, x + width, y + height);

	settextcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	int starty = y + 10;
	outtextxy(x + 15, starty, std::wstring(record.date.begin(), record.date.end()).c_str());

	std::wstring winner = record.go_first ? _T("先") : _T("后");;
	switch (record.result)
	{
	case 'r':
		winner += _T("胜");
		settextcolor(RED);
		break;
	case 'b':
		winner += _T("败");
		settextcolor(PURPLE);
		break;
	case 'n':
		winner += _T("和");
		settextcolor(DEEP_GREEN);
		break;
	}
	outtextxy(x + 190, starty, winner.c_str());

	settextcolor(PublicResource::winway_to_color.at(record.reason));
	outtextxy(x + 280, starty, record.reason.c_str());

	x += 442;
	int startx = x - 17;
	starty = y + height / 2;

	setlinestyle(PS_SOLID, 3);
	bool have_ticket = std::find(record.tickets.begin(), record.tickets.end(), true) != record.tickets.end();;
	if (have_ticket)
	{
		for (int i = 0; i < record.tickets.size(); i++)
		{
			if (record.tickets[i])
			{
				setfillcolor(PublicResource::ticket_colors[i]);
				solidcircle(x, starty, 8);
				x += 30;
			}
		}
		
		rectangle(startx, y + 5, startx + 125, y + height - 5);
	}
	else
	{
		draw_texture_in_box_center(startx, y + 5, startx + 125, y + height - 5, 25, _T("无标签"), _T("楷体"));
	}

	int buttonx = 575;
	setfillcolor(button_color);
	setlinestyle(PS_SOLID, 5);
	draw_texture_in_box_center(buttonx, y, 20 + width, y + height, 30, func_name.c_str(), _T("黑体"), BLACK, true);
}

void Renderer::draw_manage_interface_tickets(std::vector<bool>& tickets)
{
	draw_start();
	draw_manage_interface_tickets_subwindow();
	draw_manage_interface_ticket_showtickets(tickets);
	draw_end();
}

void Renderer::draw_settings_interface(size_t amount, bool go_first, bool auto_add, const std::vector<size_t>& pieces_num, const std::vector<coordinate>& selected, bool change)
{
	draw_start();
	clear();
	draw_settings_interface_frame();
	draw_settings_interface_slider(amount);
	draw_settings_interface_radiob(go_first);
	draw_settings_interface_checkb(auto_add);
	draw_game(false, 0.6f, 0, 300, true);
	draw_settings_interface_pieces(pieces_num);
	draw_settings_interface_selected(selected);
	draw_settings_interface_buttons(change);
	draw_end();
}

void Renderer::draw_settings_interface_reminder()
{
	int subwindow_width = 500;
	int subwindow_height = 340;
	int subwindow_x = (WINDOW_WIDTH - subwindow_width) / 2;
	int subwindow_y = (WINDOW_HEIGHT - subwindow_height) / 2;
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 4);
	setfillcolor(BG_COLOR);
	fillrectangle(subwindow_x, subwindow_y, subwindow_x + subwindow_width, subwindow_y + subwindow_height);

	std::wstring title = _T("警告");
	settextcolor(BLACK);
	settextstyle(40, 0, _T("黑体"));
	int title_x = subwindow_x + (subwindow_width - textwidth(title.c_str())) / 2;
	int title_y = subwindow_y + 20;
	outtextxy(title_x, title_y, title.c_str());

	std::vector<std::wstring> body =
	{
		_T("您确定要退出设置吗？"),
		_T("您的更改将不会被保存！")
	};
	settextstyle(35, 0, _T("黑体"));
	int body_y = 360;
	int cnt = 0;
	for (const std::wstring& text : body)
	{
		outtextxy(subwindow_x + (subwindow_width - textwidth(text.c_str())) / 2, body_y + cnt++ * 50, text.c_str());
	}

	int edge = 20;
	int button_width = (subwindow_width - edge * 3) / 2;
	int button_height = 50;
	int button_x = subwindow_x + edge;
	int button_y = subwindow_y + subwindow_height - edge - button_height;
	setlinestyle(PS_SOLID, 3);
	draw_texture_in_box_center(button_x, button_y, button_x + button_width, button_y + button_height,
		35, _T("退出"), _T("黑体"));
	button_x += button_width + edge;
	draw_texture_in_box_center(button_x, button_y, button_x + button_width, button_y + button_height,
		35, _T("取消"), _T("黑体"));
}

void Renderer::draw_settings_interface_frame()
{
	// Draw title
	std::wstring text = _T("设置");
	settextcolor(BLACK);
	settextstyle(60, 0, _T("宋体"));
	int textWidth = textwidth(text.c_str());
	int x = (WINDOW_WIDTH - textWidth) / 2;
	int y = 50;
	outtextxy(x, y, text.c_str());

	// Draw settings
	settextstyle(30, 0, _T("楷体"));

	text = _T("Ai算力值");
	x = 50;
	y = 180;
	outtextxy(x, y, text.c_str());

	text = _T("选择一方");
	y += 80;
	outtextxy(x, y, text.c_str());


	text = _T("自动将对局");
	x += 370;
	y -= 15;
	outtextxy(x, y, text.c_str());

	text = _T("加入我的列表");
	x -= 12;
	y += 30;
	outtextxy(x, y, text.c_str());
}

void Renderer::draw_settings_interface_slider(size_t amount)
{
	int x = 220;
	int y = 195;
	int slider_size = 420;
	setlinecolor(SLIDER_COLOR1);
	setlinestyle(PS_SOLID, 9);
	line(x - 5, y, x + slider_size + 5, y);

	int position = 0;
	int tile = slider_size / ((int)PublicResource::settings_interface_slider_info.size() - 1);
	setlinecolor(SLIDER_COLOR1);
	switch (amount)
	{
	case 5000:
		break;
	case 10000:
		position = 1;
		break;
	case 100000:
		position = 2;
		break;
	case 1000000:
		position = 3;
		break;
	case 10000000:
		position = 4;
		break;
	case 100000000:
		position = 5;
		break;
	case 500000000:
		position = 6;
		break;
	}
	int circle_x = x + position * tile;
	setfillcolor(SLIDER_COLOR2);
	solidcircle(circle_x, y, 12);
	
	settextstyle(12, 0, _T("宋体"));
	for (int i = 0; i < PublicResource::settings_interface_slider_info.size(); i++)
	{
		if (PublicResource::settings_interface_slider_values[i] == amount)
			settextcolor(SLIDER_COLOR2);
		else
			settextcolor(SLIDER_COLOR1);
		outtextxy(x + i * tile - 12, y - 32, PublicResource::settings_interface_slider_info[i].c_str());
	}
}

void Renderer::draw_settings_interface_radiob(bool go_first)
{
	int x = 200;
	int y = 250;
	int width = 180;
	int height = 50;
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(SETTINGS_CHECKB_COLOR);
	fillrectangle(x, y, x + width, y + height);

	int tile = width / 4;
	int addtion = !go_first * tile * 2;
	setfillcolor(go_first ? RED : BLACK);
	solidrectangle(x + addtion + 5, y + 5, x + addtion + width / 2 - 5, y + height - 5);

	int text_size = 30;
	settextstyle(text_size, 0, _T("黑体"));
	settextcolor(go_first ? WHITE : BLACK);
	outtextxy(x + tile - text_size / 2, y + height / 2 - text_size / 2, _T('红'));

	settextcolor(go_first ? BLACK : WHITE);
	outtextxy(x + tile * 3 - text_size / 2, y + height / 2 - text_size / 2, _T('黑'));

}

void Renderer::draw_settings_interface_checkb(bool auto_add)
{
	int x = 610;
	int y = 250;
	int size = 50;
	setlinecolor(BLACK);
	setfillcolor(SETTINGS_CHECKB_COLOR);
	setlinestyle(PS_SOLID, 3);
	fillrectangle(x, y, x + size, y + size);
	if (auto_add)
	{
		setlinecolor(GREEN);
		setlinestyle(PS_SOLID, 4);
		int edge = 10;
		line(x + edge, y + size / 7 * 3 + edge, x + size / 3 + edge, y + size - edge);
		line(x + size / 3 + edge, y + size - edge, x + size - edge, y + edge);
	}
}

void Renderer::draw_settings_interface_pieces(const std::vector<size_t>& pieces_num)
{
	chessboard_size = 0.9f;

	int start_x = 500;
	int start_y = 360;
	int interval = 70;

	int cnt = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			int curr_x = start_x + i * interval;
			int curr_y = start_y + j * interval;
			std::string curr_piece = PublicResource::piece_type[i * 7 + j];
			draw_piece(curr_x, curr_y, curr_piece);
			setfillcolor(RED);
			solidcircle(curr_x + 20, curr_y - 17, 10);
			settextcolor(WHITE);
			settextstyle(16, 0, _T("宋体"));
			outtextxy(curr_x + 17, curr_y - 25, std::to_wstring(pieces_num[cnt]).c_str());
			cnt++;
		}
	}
}

void Renderer::draw_settings_interface_selected(const std::vector<coordinate>& selected)
{
	setlinecolor(BLUE);
	setlinestyle(PS_SOLID, 2);
	if (!selected[0].isEmpty())
	{
		if (selected[0].y < 0)
		{
			int x = 473 + (selected[0].x / 6) * 70 - 5;
			int y = 333 + (selected[0].x % 6) * 70 - 5;
			rectangle(x, y, x + 64, y + 64);
		}
		else
		{
			int x = 32 + selected[0].x * 48;
			int y = 332 + selected[0].y * 48;
			rectangle(x, y, x + 36, y + 36);
		}
	}
	
	if (!selected[1].isEmpty())
	{
		int x = 32 + selected[1].x * 48;
		int y = 332 + selected[1].y * 48;
		rectangle(x, y, x + 36, y + 36);
	}
}

void Renderer::draw_settings_interface_buttons(bool change)
{
	for (int y = 0; y < 3; y++)
	{
		draw_settings_interface_button_square(620, 360 + y * 120, 
			PublicResource::settings_interface_button_colors.at(y),
			PublicResource::settings_interface_button_texts.at(y).c_str());
	}
	int x = 470;
	int y = 750;
	int width = 120;
	int height = 50;
	setfillcolor(change ? SETTINGS_SAVEB_COLOR1 : SETTINGS_SAVEB_COLOR2);
	draw_texture_in_box_center(x, y, x + width, y + height, 25, _T("保存"), _T("黑体"), BLACK, true);
	x += width + 10;
	setfillcolor(SETTINGS_EXITB_COLOR);
	draw_texture_in_box_center(x, y, x + width, y + height, 25, _T("退出"), _T("黑体"), BLACK, true);
}

void Renderer::draw_settings_interface_button_square(int x, int y, COLORREF color, const std::wstring& text)
{
	int size = 100;
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(color);
	fillrectangle(x, y, x + size, y + size);

	settextcolor(BLACK);
	settextstyle(30, 0, _T("黑体"));
	int text_width = 60;
	outtextxy(x + (size - text_width) / 2, y + 20, text.substr(0, 2).c_str());
	outtextxy(x + (size - text_width) / 2, y + 50, text.substr(2, 2).c_str());
}

void Renderer::draw_load_interface_step1()
{
	draw_start();
	clear();

	// draw title
	int title_text_size = 70;
	std::wstring title = _T("第一步");
	settextcolor(BLACK);
	settextstyle(title_text_size, 0, _T("黑体"));
	int textx = (WINDOW_WIDTH - textwidth(title.c_str())) / 2;
	int texty = 70;
	outtextxy(textx, texty, title.c_str());

	// draw main body
	int body_text_size = 40;
	settextstyle(body_text_size, 0, _T("黑体"));

	std::vector<std::wstring> body = 
	{
		_T("将好友或其他人发给你的对局码"),
		_T("复制到剪切板"),
	};

	texty = 300;
	for (std::wstring& line : body)
	{
		textx = (WINDOW_WIDTH - textwidth(line.c_str())) / 2;
		outtextxy(textx, texty, line.c_str());
		texty += body_text_size + 20;
	}

	int example_text_size = 25;
	settextcolor(WHITE);
	settextstyle(example_text_size, 0, _T("楷体"));
	std::wstring example_text = _T("2024-03-31/n/0/1/0000/original/original/------");
	int example_text_width = textwidth(example_text.c_str());
	texty += 30;
	textx = (WINDOW_WIDTH - example_text_width) / 2;
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	fillrectangle(textx - 7, texty - 7, textx + example_text_width + 7, texty + example_text_size + 7);

	setfillcolor(SELECTED_COLOR);
	solidrectangle(textx, texty, textx + example_text_width, texty + example_text_size);
	outtextxy(textx, texty, example_text.c_str());

	texty += 60;
	std::wstring ctrlc_text = _T("Ctrl + C");
	settextcolor(BLACK);
	settextstyle(body_text_size, 0, _T("楷体"));
	int ctrlc_text_width = textwidth(ctrlc_text.c_str());
	textx = (WINDOW_WIDTH - ctrlc_text_width) / 2;
	outtextxy(textx, texty, ctrlc_text.c_str());

	// draw buttons
	int xedge = 80;
	int yedge = 20;
	int button_width = (WINDOW_WIDTH - xedge * 3) / 2;
	int button_height = 80;
	int button_y = WINDOW_HEIGHT - yedge - button_height;
	draw_texture_in_box_center(xedge, button_y, xedge + button_width, button_y + button_height,
		30, _T("退出"), _T("宋体"));
	xedge += xedge + button_width;
	draw_texture_in_box_center(xedge, button_y, xedge + button_width, button_y + button_height,
		30, _T("下一步"), _T("宋体"));
	draw_end();
}

void Renderer::draw_load_interface_step2(bool select, const std::wstring& info)
{
	draw_start();
	clear();

	// draw title
	int title_text_size = 70;
	std::wstring title = _T("第二步");
	settextcolor(BLACK);
	settextstyle(title_text_size, 0, _T("黑体"));
	int textx = (WINDOW_WIDTH - textwidth(title.c_str())) / 2;
	int texty = 70;
	outtextxy(textx, texty, title.c_str());

	// draw body
	int body_text_size = 40;
	settextstyle(body_text_size, 0, _T("黑体"));
	std::vector<std::wstring> body =
	{
		_T("粘贴复盘码到文本框里"),
		_T("按 ”Ctrl + V“ 粘贴信息"),
	};

	texty = 320;
	for (std::wstring& line : body)
	{
		textx = (WINDOW_WIDTH - textwidth(line.c_str())) / 2;
		outtextxy(textx, texty, line.c_str());
		texty += body_text_size + 20;
	}

	// draw text box 
	int box_text_size = 25;
	settextcolor(WHITE);
	settextstyle(box_text_size, 0, _T("楷体"));
	int box_width = 600;
	texty += 30;
	textx = (WINDOW_WIDTH - box_width) / 2;
	setlinecolor(select ? SELECTED_COLOR : BLACK);
	setfillcolor(WHITE);
	fillrectangle(textx - 7, texty - 7, textx + box_width + 7, texty + box_text_size + 7);
	if (select && !info.size())
	{
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		line(textx, texty, textx, texty + box_text_size);
	}
	if (info.size())
	{
		settextcolor(BLACK);
		settextstyle(box_text_size, 0, _T("宋体"));
		int limit = 46;
		outtextxy(textx, texty, info.substr(0, min(limit, info.size())).c_str());
	}

	// draw buttons
	int xedge = 80;
	int yedge = 20;
	int button_width = (WINDOW_WIDTH - xedge * 3) / 2;
	int button_height = 80;
	int button_y = WINDOW_HEIGHT - yedge - button_height;
	setlinestyle(PS_SOLID, 3);
	draw_texture_in_box_center(xedge, button_y, xedge + button_width, button_y + button_height,
		30, _T("上一步"), _T("宋体"));
	xedge += xedge + button_width;
	draw_texture_in_box_center(xedge, button_y, xedge + button_width, button_y + button_height,
		30, _T("完成"), _T("宋体"));

	draw_end();
}

void Renderer::draw_game_start_sign()
{
	for (const wchar_t c : _T("博弈开始"))
	{
		if (c != '\0')
		{
			this->draw_start();
			this->clear();
			this->draw_game(false);
			this->draw_sign(BLACK, c);
			this->draw_end();
			Sleep(500);
		}
	}
}

void Renderer::draw_game_with_batch(bool is_call_general)
{
	this->draw_start();
	this->clear();
	this->draw_game(is_call_general);
	this->draw_end();
}

void Renderer::draw_game(bool is_call_general, float size, int x, int y, bool view_only)
{
	chessboard_size = size;
	chessboard_x = x;
	chessboard_y = y;
	this->draw_chessBoard(view_only);
	if (!view_only)
		this->draw_tips();
	this->draw_pieces();
	if (!view_only)
		this->draw_target_sigh();
	if (is_call_general)
		this->draw_call_general_sign();
}

void Renderer::load_imgs()
{
	loadimage(&main_interface, main_interface_bk_png_path, 660, 440);
}

void Renderer::draw_start()
{
	BeginBatchDraw();
}

void Renderer::draw_end()
{
	EndBatchDraw();
}

void Renderer::clear()
{
	cleardevice();
}

void Renderer::draw_chessBoard(bool view_only)
{
	setlinecolor(BLACK);

	// Draw grid
	setlinestyle(PS_SOLID, 2);

	int edgex = chessboard_x + edge;
	int edgey = chessboard_y + edge;
	int right = chessboard_x + TCG(GRID_WIDTH - 1);
	int bottom = chessboard_y + TCG(GRID_HEIGHT - 1);

	line(edgex - 1, edgey - 1, edgex, edgey);

	for (int i = 0; i < GRID_WIDTH; i++)
	{
		line(chessboard_x + TCG(i), edgey, chessboard_x + TCG(i), bottom);
	}

	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		line(edgex, chessboard_y + TCG(i), right, chessboard_y + TCG(i));
	}

	line(chessboard_x + TCG(3), chessboard_y + TCG(0), chessboard_x + TCG(5), chessboard_y + TCG(2));
	line(chessboard_x + TCG(5), chessboard_y + TCG(0), chessboard_x + TCG(3), chessboard_y + TCG(2));

	line(chessboard_x + TCG(3), chessboard_y + TCG(7), chessboard_x + TCG(5), chessboard_y + TCG(9));
	line(chessboard_x + TCG(5), chessboard_y + TCG(7), chessboard_x + TCG(3), chessboard_y + TCG(9));

	setlinestyle(PS_SOLID, 3);

	int ti = 5;

	line(edgex - ti, edgey - ti, right + ti, edgey - ti);
	line(right + ti, edgey - ti, right + ti, bottom + ti);
	line(edgex - ti, bottom + ti, right + ti, bottom + ti);
	line(edgex - ti, edgey - ti, edgex - ti, bottom + ti);


	//Draw river
	setfillcolor(RIVER_COLOR);
	solidrectangle(edgex + 1, chessboard_y + TCG(4) + 1, right - 2, chessboard_y + TCG(5) - 2);

	//Draw flowers
	setlinestyle(PS_SOLID, 2);
	draw_chessBoard_flower(1, 2);
	draw_chessBoard_flower(7, 2);

	draw_chessBoard_flower(0, 3);
	draw_chessBoard_flower(2, 3);
	draw_chessBoard_flower(4, 3);
	draw_chessBoard_flower(6, 3);
	draw_chessBoard_flower(8, 3);

	draw_chessBoard_flower(1, 7);
	draw_chessBoard_flower(7, 7);

	draw_chessBoard_flower(0, 6);
	draw_chessBoard_flower(2, 6);
	draw_chessBoard_flower(4, 6);
	draw_chessBoard_flower(6, 6);
	draw_chessBoard_flower(8, 6);

	//Draw (chu he han jie)
	if (!view_only)
	{
		settextcolor(BLACK);
		settextstyle(50, 25, _T("楷体"));

		int textx = TCG(4) + 13;
		outtextxy(150, textx, _T("楚"));
		outtextxy(250, textx, _T("河"));
		outtextxy(440, textx, _T("汉"));
		outtextxy(540, textx, _T("界"));
	}
}

void Renderer::draw_chessBoard_flower(int x, int y)
{
	x = chessboard_x + TCG(x);
	y = chessboard_y + TCG(y);
	int interval = (int)(5 * chessboard_size);
	int length = (int)(20 * chessboard_size);
	if (x != edge)
	{
		line(x - interval - length, y - interval, x - interval, y - interval);
		line(x - interval, y - interval - length, x - interval, y - interval);
		line(x - interval - length, y + interval, x - interval, y + interval);
		line(x - interval, y + interval, x - interval, y + interval + length);
	}
	if (x != TCG(GRID_WIDTH - 1))
	{
		line(x + interval, y - interval, x + interval + length, y - interval);
		line(x + interval, y - interval - length, x + interval, y - interval);
		line(x + interval, y + interval, x + interval + length, y + interval);
		line(x + interval, y + interval, x + interval, y + interval + length);
	}
}

void Renderer::draw_pieces()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			std::string name = (*chess_board)[i][j];
			if (name != "sp")
			{
				int x = chessboard_x + TCG(j);
				int y = chessboard_y + TCG(i);

				draw_piece(x, y, name);
			}
		}
	}
}

void Renderer::draw_piece(int x, int y, const std::string& type)
{
	char color = type[0];
	const wchar_t text = PublicResource::piece_name_translater.at(type);
	int piece_size = (int)(PIECE_SIZE * chessboard_size);

	setlinestyle(PS_SOLID, 2);
	setfillcolor(PIECE_COLOR);
	setlinecolor(BLACK);
	fillcircle(x, y, piece_size / 2);

	setlinecolor(ORANGE);
	setlinestyle(PS_SOLID, (int)(4 * chessboard_size));
	circle(x, y, piece_size / 2 - (int)(8 * chessboard_size));

	settextstyle(int(30 * chessboard_size), 0, _T("楷体"));
	settextcolor((color == 'r') ^ !PublicResource::current_go_first ? RED : BLACK);
	outtextxy(x - piece_size / 4, y - piece_size / 4, text);
}

void Renderer::draw_tips()
{
	int half = PIECE_SIZE / 2;
	if (!select_piece->isEmpty()) 
	{
		int x = TCG(select_piece->x);
		int y = TCG(select_piece->y);
		setlinecolor(BLUE);
		rectangle(x - half - 5, y - half - 5, x + half + 5, y + half + 5);
	}

	coordinate* temp = last_move_begin_coordinate;
	for (int i = 0; i < 2; i++)
	{
		if (!temp->isEmpty()) 
		{
			int x = TCG(temp->x);
			int y = TCG(temp->y);
			setfillcolor(GRAY);
			drawSemiTransparentRect(x - half - 5, y - half - 5, PIECE_SIZE + 10, PIECE_SIZE + 10, 150);
		}

		temp = last_move_end_coordinate;
	}
}

void Renderer::draw_target_sigh()
{
	setfillcolor(GREEN);
	for (const cma::coordinate& coord : *destination_set)
	{
		solidcircle(TCG(coord.x), TCG(coord.y), 10);
	}
}

void Renderer::draw_call_general_sign()
{
	this->draw_sign(BLACK, _T('将'));
}

void Renderer::draw_game_final_sign(char winner)
{
	this->draw_start();
	this->draw_sign(winner == 'r' ? RED : (winner == 'b' ? BLACK : DEEP_GREEN),
		(winner == 'r' ? _T('胜') : (winner == 'b' ? _T('负') : _T('和'))));
	this->draw_end();
}

void Renderer::draw_frame(int x_edge, int y_edge, int box_size)
{
	setlinecolor(BLACK);
	setfillcolor(BOX_COLOR);
	setlinestyle(PS_SOLID, 5);
	fillrectangle(x_edge, y_edge, x_edge + box_size, y_edge + box_size);
}

void Renderer::draw_title(int x_edge, int y_edge, int box_size, char winner)
{
	int text_length = winner == 'n' ? 2 : 3;
	int text_size = 50;
	int title_x_size = text_size * text_length + 20;
	int title_y_size = 70;
	int box_x_edge = (box_size - title_x_size) / 2;
	int box_y_edge = 30;
	setlinestyle(PS_SOLID, 5);
	draw_texture_in_box_center(x_edge + box_x_edge, y_edge + box_y_edge,
		x_edge + box_x_edge + title_x_size, y_edge + box_y_edge + title_y_size,
		text_size, winner == 'r' ? _T("你赢了") : 
		(winner == 'b' ? _T("你输了") : _T("平局")), _T("黑体"));
}

void Renderer::draw_winway(int x_edge, int y_edge, int box_size, const wchar_t* win_way)
{
	int text_size = 75;
	int text_length = (int)wcslen(win_way);
	int text_width = text_length * text_size;
	int text_x_edge = (box_size - text_width) / 2;

	COLORREF color = PublicResource::winway_to_color.at(std::wstring(win_way));
	int x = x_edge + text_x_edge;
	int y = y_edge + 180;
	setlinestyle(PS_SOLID, 8);
	draw_texture_in_box_center(x - 10, y - 10, x + text_width + 10, 
		y + text_size + 10, text_size, win_way, _T("黑体"), color);
}

void Renderer::draw_button(int x_edge, int y_edge, int box_size)
{
	int button_width = (box_size - 30) / 2;
	int button_heighth = 70;

	int left = x_edge + 10;
	int botton = y_edge + box_size - 10;

	int lx = left + button_width;
	int mx = lx + 10;
	int rx = mx + button_width;

	int ly = botton - button_heighth;
	int my = ly - 10;
	int hy = my - button_heighth;

	int text_size = 40;
	const wchar_t* font = _T("黑体");

	setlinestyle(PS_SOLID, 5);
	draw_texture_in_box_center(left, hy, lx, my, text_size, _T("重新开始"), font);
	draw_texture_in_box_center(mx, hy, rx, my, text_size, _T("查看结局"), font);
	draw_texture_in_box_center(left, ly, lx, botton, text_size, _T("复盘"), font);
	draw_texture_in_box_center(mx, ly, rx, botton, text_size, _T("回到主页"), font);
}

void Renderer::draw_game_renew(char winner, std::wstring win_way)
{
	this->draw_start();
	this->clear();
	this->draw_game(false);
	int box_size = 500;
	int x_edge = (WINDOW_WIDTH - box_size) / 2;
	int y_edge = (WINDOW_HEIGHT - box_size) / 2;

	draw_frame(x_edge, y_edge, box_size);
	draw_title(x_edge, y_edge, box_size, winner);
	draw_winway(x_edge, y_edge, box_size, win_way.c_str());
	draw_button(x_edge, y_edge, box_size);

	this->draw_end();
}

void Renderer::draw_game_record(int pointer, size_t record_size)
{
	this->draw_start();
	this->clear();
	draw_game(false);
	if (pointer < 0) draw_message_box(_T("已经是第一页"));
	else if (pointer >= record_size) draw_message_box(_T("已经是最后一页"));
	this->draw_end();
}

void Renderer::draw_message_box(std::wstring text)
{
	int text_length = (int)text.size();
	int text_size = 75;
	int width = text_length * text_size;
	int text_x_edge = (WINDOW_WIDTH - width) / 2;
	int text_y_edge = (WINDOW_HEIGHT - (text_size)) / 2;
	setfillcolor(BOX_COLOR);
	setlinestyle(PS_SOLID, 3);
	solidrectangle(text_x_edge - 10, text_y_edge - 10,
		text_x_edge + width + 10, text_y_edge + text_size + 10);
	draw_texture_in_box_center(text_x_edge - 10, text_y_edge - 10,
		text_x_edge + width + 10, text_y_edge + text_size + 10,
		text_size, text.c_str(), _T("黑体"), BLACK);
}

void Renderer::draw_sign(unsigned long color, const wchar_t text)
{
	setlinecolor(color);
	setlinestyle(PS_SOLID, 10);
	circle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 70);

	settextstyle(100, 0, _T("黑体"));
	settextcolor(color);
	outtextxy(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50, text);
}

void Renderer::draw_texture_in_box_center(int x1, int y1, int x2, int y2, int text_size, const wchar_t* text, const wchar_t* font, unsigned long color, bool fill)
{
	setlinecolor(color);
	if (fill)
		fillrectangle(x1, y1, x2, y2);
	else
		rectangle(x1, y1, x2, y2);

	settextstyle(text_size, 0, font);
	settextcolor(color);

	int text_x_edge = ((x2 - x1) - textwidth(text)) / 2;
	int text_y_edge = ((y2 - y1) - (textheight(text))) / 2;

	outtextxy(x1 + text_x_edge, y1 + text_y_edge, text);
}

void Renderer::drawSemiTransparentRect(int x, int y, int width, int height, int alpha)
{
	COLORREF originalColor = getfillcolor();

	for (int i = x; i < x + width; ++i) {
		for (int j = y; j < y + height; ++j) {
			COLORREF pixelColor = getpixel(i, j);

			int blendedRed = GetRValue(originalColor) * (255 - alpha) / 255 + GetRValue(pixelColor) * alpha / 255;
			int blendedGreen = GetGValue(originalColor) * (255 - alpha) / 255 + GetGValue(pixelColor) * alpha / 255;
			int blendedBlue = GetBValue(originalColor) * (255 - alpha) / 255 + GetBValue(pixelColor) * alpha / 255;

			putpixel(i, j, RGB(blendedRed, blendedGreen, blendedBlue));
		}
	}
}

void Renderer::draw_manage_interface_tickets_subwindow()
{
	// draw subwindow 
	setfillcolor(BG_COLOR);
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 5);
	int subwindow_width = 500;
	int subwindow_height = 340;
	int edgex = (WINDOW_WIDTH - subwindow_width) / 2;
	int edgey = (WINDOW_HEIGHT - subwindow_height) / 2;
	fillrectangle(edgex, edgey, edgex + subwindow_width, edgey + subwindow_height);

	// draw exit button
	setlinestyle(PS_SOLID, 3);
	int button_size = 50;
	int button_edge = 7;
	rectangle(edgex + button_edge, edgey + button_edge, edgex + button_edge + button_size, edgey + button_edge + button_size);
	
	int Xedge = 8;
	int Xx = edgex + button_edge + Xedge;
	int Xy = edgey + button_edge + Xedge;
	line(Xx, Xy, Xx + button_size - Xedge * 2, Xy + button_size - Xedge * 2);
	line(Xx + button_size - Xedge * 2, Xy, Xx, Xy + button_size - Xedge * 2);

}

void Renderer::draw_manage_interface_ticket_showtickets(std::vector<bool>& tickets)
{
	// draw tickets ( examples )
	int start_x = 205;
	int start_y = 400;
	int interval = 110;
	int box_size = 90;
	settextcolor(BLACK);
	settextstyle(20, 0, _T("楷体"));
	setlinecolor(BLUE);
	int text_size = textwidth(PublicResource::manage_interface_ticket_types[0].c_str());
	for (int x = 0; x < PublicResource::ticket_colors.size(); x++)
	{
		int ticketx = start_x + x * interval;
		setfillcolor(SHADOW_COLOR);
		solidrectangle (ticketx - box_size / 2, start_y - box_size / 2, ticketx + box_size / 2, start_y + box_size / 2);
		if (tickets[x])
		{
			rectangle(ticketx - box_size / 2 - 5, start_y - box_size / 2 - 5, 
				ticketx + box_size / 2 + 5, start_y + box_size / 2 + 5);
		}
		setfillcolor(PublicResource::ticket_colors[x]);
		solidcircle(ticketx, start_y, 18);
		outtextxy(ticketx - text_size / 2, start_y + 70, PublicResource::manage_interface_ticket_types[x].c_str());
	}
}

