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

	void draw_game_with_batch(bool is_call_general);
	void draw_game_final_sign(char winner);
	void draw_game_renew(char winner, std::wstring win_way);
	void draw_game_record(int pointer, size_t record_size);

	void draw_main_interface();

	void draw_settings_interface(size_t amount, bool go_first, bool auto_add, const std::vector<size_t>& pieces_num, const std::vector<coordinate>&selected, bool change);
	void draw_settings_interface_reminder();

	void draw_load_interface_step1();
	void draw_load_interface_step2(bool select, const std::wstring& info);

	void draw_manage_interface_carte(size_t carte_id, size_t func_id, const std::vector<std::vector<std::wstring>>& functions, const std::vector<std::vector<COLORREF>>& colors);
	void draw_manage_interface_records(std::wstring func_name, std::vector<Record>& records, std::vector<std::pair<Record, Record>> linked_record, size_t page, std::wstring button_lable, COLORREF button_color);
	void draw_manage_interface_tickets(std::vector<bool>& tickets);

	void draw_game_start_sign();
	void draw_message_box(std::wstring text);
private:
	int edge;
	int spacing;
	CHESS_BOARD* chess_board;
	coordinate* select_piece;
	coordinate* last_move_begin_coordinate;
	coordinate* last_move_end_coordinate;
	CMA_RESULT_SET* destination_set;
	float chessboard_size;
	int chessboard_x;
	int chessboard_y;

	IMAGE main_interface;
private:
	void load_imgs();

	void draw_start();
	void draw_end();
	void clear();
	void draw_chessBoard(bool view_only);
	void draw_chessBoard_flower(int x, int y);
	void draw_pieces();
	void draw_piece(int x, int y, const std::string& type);
	void draw_tips();
	void draw_target_sigh();
	void draw_call_general_sign();

	void draw_game(bool is_call_general, float size = 1.0f, int x = 0, int y = 0, bool view_only = false);
	void draw_frame(int x_edge, int y_edge, int box_size);
	void draw_title(int x_edge, int y_edge, int box_size, char winner);
	void draw_winway(int x_edge, int y_edge, int box_size, const wchar_t* win_way);
	void draw_button(int x_edge, int y_edge, int box_size);

	void draw_sign(unsigned long color, const wchar_t text);
	void draw_texture_in_box_center(int x1, int y1, int x2, int y2, int text_size, const wchar_t* text, const wchar_t* font, unsigned long color = BLACK, bool fill = false);
	void drawSemiTransparentRect(int x, int y, int width, int height, int alpha);

	void draw_manage_interface_tickets_subwindow();
	void draw_manage_interface_ticket_showtickets(std::vector<bool>& tickets);

	void draw_manage_interface_record(int x, int y, std::wstring func_name, COLORREF color, Record& record);

	void draw_settings_interface_frame();
	void draw_settings_interface_slider(size_t amount);
	void draw_settings_interface_radiob(bool go_first);
	void draw_settings_interface_checkb(bool auto_add);
	void draw_settings_interface_pieces(const std::vector<size_t>& pieces_num);
	void draw_settings_interface_selected(const std::vector<coordinate>& selected);
	void draw_settings_interface_buttons(bool change);
	void draw_settings_interface_button_square(int x, int y, COLORREF color, const std::wstring& text);
};

