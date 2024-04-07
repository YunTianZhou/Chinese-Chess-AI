#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <easyx.h>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <random>
namespace fs = std::filesystem;

#include "CMA.h"
#include "Coordinate.h"
#include "Record.h"


using CHESS_BOARD = std::vector<std::vector<std::string>>;

const int WINDOW_WIDTH = 740;
const int WINDOW_HEIGHT = 820;
const int GRID_WIDTH = 9;
const int GRID_HEIGHT = 10;
const int PIECE_SIZE = 60;

const COLORREF BG_COLOR = RGB(248, 207, 119);
const COLORREF RIVER_COLOR = RGB(50, 140, 230);
const COLORREF PIECE_COLOR = RGB(243, 231, 137);
const COLORREF BOX_COLOR = RGB(233, 216, 133);
const COLORREF GRAY = RGB(100, 100, 100);
const COLORREF ORANGE = RGB(243, 173, 29);
const COLORREF DEEP_GREEN = RGB(0, 128, 0);
const COLORREF PURPLE = RGB(123, 31, 162);
const COLORREF SHADOW_COLOR = RGB(181, 158, 110);
const COLORREF SELECTED_COLOR = RGB(0, 120, 215);
const COLORREF SLIDER_COLOR1 = RGB(165, 148, 141);
const COLORREF SLIDER_COLOR2 = RGB(109, 79, 77);
const COLORREF SETTINGS_CHECKB_COLOR = RGB(255, 234, 183);
const COLORREF SETTINGS_SAVEB_COLOR1 = RGB(0, 191, 255);
const COLORREF SETTINGS_SAVEB_COLOR2 = RGB(169, 169, 169);
const COLORREF SETTINGS_EXITB_COLOR = RGB(255, 165, 0);

const std::string chessboard_txt_path = "./res/chessboard.txt";
const std::string original_chessBoard_txt_path = "./res/original_chessboar.txt";
const std::string amount_txt_path = "./res/amount.txt";
const std::string curr_review_code_txt_path = "./res/curr_review_code.txt";
const std::string go_first_txt_path = "./res/go_first.txt";
const std::string automatically_add_txt_path = "./res/automatically_add.txt";
const LPCWSTR main_interface_bk_png_path = _T("./res/main_interface_bk.png");
const LPCWSTR chess_ico_path = _T("./res/chess.ico");

const std::string recent_record_list_path = "./res/recent_record_list";
const std::string my_record_list_path = "./res/my_record_list";
const std::string load_record_list_path = "./res/load_record_list";

const int recent_record_limit = 16;
const int record_page_limit = 8;
const float not_eat_times_limit = 50.0f;