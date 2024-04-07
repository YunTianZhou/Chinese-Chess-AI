#pragma once
#include "Headers.h"

namespace PublicResource
{
	extern CHESS_BOARD current_chessBoard;
	extern CHESS_BOARD current_original_chessBoard;
	extern size_t current_amount;
	extern bool current_go_first;
	extern bool current_automatically_add;
	extern int current_recent_id;
	extern int current_my_id;
	extern int current_load_id;

	extern cma::ChessMovesAnalyze<std::string>* Cma;

	extern const std::unordered_map<char, std::vector<
		std::vector<int>>> piece_position_score;
	extern const std::vector<std::pair<coordinate, coordinate>> first_move;
	extern const std::vector<std::string> piece_type;
	extern const std::unordered_map<std::wstring, COLORREF> winway_to_color;
	extern const std::unordered_map<int, std::wstring> code_to_ticket;
	extern const std::vector<COLORREF> ticket_colors;
	extern const std::unordered_map<std::wstring, int> explaintion_to_code;
	extern const std::unordered_map<int, std::wstring> code_to_explaintion;
	extern const std::unordered_map<std::string, const wchar_t> piece_name_translater;
	extern const std::unordered_map<char, int> piece_power;
	extern const std::unordered_map<char, int> piece_activity;

	extern const std::vector<std::vector<std::wstring>> manage_interface_functions;
	extern const std::unordered_map<size_t, std::wstring> manage_interface_func_to_label;
	extern const std::vector<std::vector<COLORREF>> manage_interface_colors;
	extern const std::vector<std::wstring> manage_interface_ticket_types;
	extern const std::vector<std::wstring> settings_interface_slider_info;
	extern const std::vector<size_t> settings_interface_slider_values;
	extern const std::unordered_map<char, std::vector<
		std::vector<bool>>> settings_interface_piece_legal_position;
	extern std::vector<std::wstring> settings_interface_button_texts;
	extern std::vector<COLORREF> settings_interface_button_colors;
}

