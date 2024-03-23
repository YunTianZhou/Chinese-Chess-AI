#pragma once
#include "Headers.h"

class Renderer
{
public:
	Renderer();
	~Renderer();
	void Bind(CHESS_BOARD* chess_board,
		coordinate* select_piece,
		coordinate* last_move_begin_coordinate,
		coordinate* last_move_end_coordinate,
		CMA_RESULT_SET* destination_set);
	void draw_start_sign();
	void draw(bool is_call_general);
	void draw_final_sign(char winner);
	void draw_renew(char winner, std::wstring win_way);
	void draw_record(int pointer, size_t record_size);
	void draw_record_sign(std::wstring text);
private:
	int edge;
	int spacing;
	CHESS_BOARD* chess_board;
	coordinate* select_piece;
	coordinate* last_move_begin_coordinate;
	coordinate* last_move_end_coordinate;
	CMA_RESULT_SET* destination_set;
private:
	void draw_start();
	void draw_end();
	void clear();
	void draw_chessBoard();
	void draw_chessBoard_flower(int x, int y);
	void draw_pieces();
	void draw_tips();
	void draw_target_sigh();
	void draw_call_general_sign();

	void draw_game(bool is_call_general);
	void draw_frame(int x_edge, int y_edge, int box_size);
	void draw_title(int x_edge, int y_edge, int box_size, char winner);
	void draw_winway(int x_edge, int y_edge, int box_size, const wchar_t* win_way);
	void draw_button(int x_edge, int y_edge, int box_size);

	void draw_sign(unsigned long color, const wchar_t text);
	void draw_texture_in_box_center(int x1, int y1, int x2, int y2, int text_length, int text_size, const wchar_t* text, const wchar_t* font, unsigned long color = BLACK);
	void drawSemiTransparentRect(int x, int y, int width, int height, int alpha);
};

