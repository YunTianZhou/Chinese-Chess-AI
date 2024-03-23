#include "Renderer.h"
#include "tool.h"

#define TCG(xy) (edge + spacing * (xy))

Renderer::Renderer() 
	: edge(50), spacing((WINDOW_HEIGHT - edge * 2) / (GRID_HEIGHT - 1)),
	  chess_board(nullptr), select_piece(nullptr),
	  last_move_begin_coordinate(nullptr),
	  last_move_end_coordinate(nullptr),
	  destination_set(nullptr)
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
	LOG("RENDER STARTING!");
}

Renderer::~Renderer()
{
	closegraph();
	LOG("RENDER ENDING!");
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

void Renderer::draw_start_sign()
{
	for (const wchar_t c : _T("博弈开始"))
	{
		if (c != '\0')
		{
			this->draw_start();
			this->draw_game(false);
			this->draw_sign(BLACK, c);
			this->draw_end();
			Sleep(500);
		}
	}
}

void Renderer::draw(bool is_call_general)
{
	this->draw_start();
	this->draw_game(is_call_general);
	this->draw_end();
}

void Renderer::draw_game(bool is_call_general)
{
	this->clear();
	this->draw_chessBoard();
	this->draw_tips();
	this->draw_pieces();
	this->draw_target_sigh();
	if (is_call_general)
		this->draw_call_general_sign();
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

void Renderer::draw_chessBoard()
{
	setlinecolor(BLACK);

	// Draw grid
	setlinestyle(PS_SOLID, 2);

	int right = TCG(GRID_WIDTH - 1);
	int botton = TCG(GRID_HEIGHT - 1);

	line(edge - 1, edge - 1, edge, edge);

	for (int i = 0; i < GRID_WIDTH; i++)
	{
		line(TCG(i), edge, TCG(i), botton);
	}

	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		line(edge, TCG(i), right, TCG(i));
	}

	line(TCG(3), TCG(0), TCG(5), TCG(2));
	line(TCG(5), TCG(0), TCG(3), TCG(2));

	line(TCG(3), TCG(7), TCG(5), TCG(9));
	line(TCG(5), TCG(7), TCG(3), TCG(9));

	setlinestyle(PS_SOLID, 3);

	int ti = 5;

	line(edge - ti, edge - ti, right + ti, edge - ti);
	line(right + ti, edge - ti, right + ti, botton + ti);
	line(edge - ti, botton + ti, right + ti, botton + ti);
	line(edge - ti, edge - ti, edge - ti, botton + ti);


	//Draw river
	setfillcolor(RIVER_COLOR);
	solidrectangle(edge + 1, TCG(4) + 1, right - 2, TCG(5) - 2);

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
	settextcolor(BLACK);
	settextstyle(50, 25, _T("楷体"));

	int textx = TCG(4) + 13;
	outtextxy(150, textx, _T("楚"));
	outtextxy(250, textx, _T("河"));
	outtextxy(440, textx, _T("汉"));
	outtextxy(540, textx, _T("界"));
}

void Renderer::draw_chessBoard_flower(int x, int y)
{
	x = TCG(x);
	y = TCG(y);
	int interval = 5;
	int length = 20;
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
				int x = TCG(j);
				int y = TCG(i);

				char color = name[0];
				const wchar_t text = tool::translater[name];

				setlinestyle(PS_SOLID, 2);
				setfillcolor(PIECE_COLOR);
				setlinecolor(BLACK);
				fillcircle(x, y, PIECE_SIZE / 2);

				setlinecolor(ORANGE);
				setlinestyle(PS_SOLID, 4);
				circle(x, y, PIECE_SIZE / 2 - 8);

				settextstyle(30, 0, _T("楷体"));
				settextcolor(color == 'r' ? RED : BLACK);
				outtextxy(x - PIECE_SIZE / 4, y - PIECE_SIZE / 4, text);
			}
		}
	}
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

void Renderer::draw_final_sign(char winner)
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
		text_length, text_size, winner == 'r' ? _T("你赢了") : 
		(winner == 'b' ? _T("你输了") : _T("平局")), _T("黑体"));
}

void Renderer::draw_winway(int x_edge, int y_edge, int box_size, const wchar_t* win_way)
{
	int text_size = 75;
	int text_length = (int)wcslen(win_way);
	int text_width = text_length * text_size;
	int text_x_edge = (box_size - text_width) / 2;

	unsigned long color;
	if (!wcscmp(win_way, _T("红方绝杀"))) color = RED;
	else if (!wcscmp(win_way, _T("黑方绝杀"))) color = BLACK;
	else if (!wcscmp(win_way, _T("和棋"))) color = DEEP_GREEN;
	else if (!wcscmp(win_way, _T("黑方掀棋盘"))) color = PURPLE;
	else color = GRAY;
	int x = x_edge + text_x_edge;
	int y = y_edge + 180;
	setlinestyle(PS_SOLID, 8);
	draw_texture_in_box_center(x - 10, y - 10, x + text_width + 10, 
		y + text_size + 10, text_length, text_size, win_way, _T("黑体"), color);
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
	draw_texture_in_box_center(left, hy, lx, my, 4, text_size, _T("重新开始"), font);
	draw_texture_in_box_center(mx, hy, rx, my, 4, text_size, _T("查看结局"), font);
	draw_texture_in_box_center(left, ly, lx, botton, 2, text_size, _T("复盘"), font);
	draw_texture_in_box_center(mx, ly, rx, botton, 3, text_size, _T("不玩了"), font);
}

void Renderer::draw_renew(char winner, std::wstring win_way)
{
	this->draw_start();
	draw_game(false);
	int box_size = 500;
	int x_edge = (WINDOW_WIDTH - box_size) / 2;
	int y_edge = (WINDOW_HEIGHT - box_size) / 2;

	draw_frame(x_edge, y_edge, box_size);
	draw_title(x_edge, y_edge, box_size, winner);
	draw_winway(x_edge, y_edge, box_size, win_way.c_str());
	draw_button(x_edge, y_edge, box_size);

	this->draw_end();
}

void Renderer::draw_record(int pointer, size_t record_size)
{
	this->draw_start();
	draw_game(false);
	if (pointer < 0) draw_record_sign(_T("已经是第一页"));
	else if (pointer >= record_size) draw_record_sign(_T("已经是最后一页"));
	this->draw_end();
}

void Renderer::draw_record_sign(std::wstring text)
{
	int text_length = (int)text.size();
	int text_size = 75;
	int width = text_length * text_size;
	int text_x_edge = (WINDOW_WIDTH - width) / 2;
	int text_y_edge = (WINDOW_HEIGHT - (text_size)) / 2;
	setfillcolor(BOX_COLOR);
	solidrectangle(text_x_edge - 10, text_y_edge - 10,
		text_x_edge + width + 10, text_y_edge + text_size + 10);
	draw_texture_in_box_center(text_x_edge - 10, text_y_edge - 10,
		text_x_edge + width + 10, text_y_edge + text_size + 10,
		text_length, text_size, text.c_str(), _T("黑体"), BLACK);
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

void Renderer::draw_texture_in_box_center(int x1, int y1, int x2, int y2, int text_length, int text_size, const wchar_t* text, const wchar_t* font, unsigned long color)
{
	setlinecolor(color);
	rectangle(x1, y1, x2, y2);

	settextstyle(text_size, 0, font);
	settextcolor(color);

	int text_x_edge = ((x2 - x1) - (text_length * text_size)) / 2;
	int text_y_edge = ((y2 - y1) - (text_size)) / 2;

	outtextxy(x1 + text_x_edge, y1 + text_y_edge, text);
}

void Renderer::drawSemiTransparentRect(int x, int y, int width, int height, int alpha)
{
	// 获取当前画笔颜色
	COLORREF originalColor = getfillcolor();

	// 遍历矩形区域的像素，并设置半透明效果
	for (int i = x; i < x + width; ++i) {
		for (int j = y; j < y + height; ++j) {
			COLORREF pixelColor = getpixel(i, j);

			// 计算混合后的颜色
			int blendedRed = GetRValue(originalColor) * (255 - alpha) / 255 + GetRValue(pixelColor) * alpha / 255;
			int blendedGreen = GetGValue(originalColor) * (255 - alpha) / 255 + GetGValue(pixelColor) * alpha / 255;
			int blendedBlue = GetBValue(originalColor) * (255 - alpha) / 255 + GetBValue(pixelColor) * alpha / 255;

			// 设置当前像素颜色
			putpixel(i, j, RGB(blendedRed, blendedGreen, blendedBlue));
		}
	}
}