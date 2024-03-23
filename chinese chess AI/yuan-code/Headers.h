#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <easyx.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <algorithm>

#include "CMA.h"
#include "Coordinate.h"

#define LOG(message) std::cout << "Log : " << message << std::endl

#define CHESS_BOARD std::vector<std::vector<std::string>>

#define WINDOW_WIDTH 740
#define WINDOW_HEIGHT 820
#define GRID_WIDTH 9
#define GRID_HEIGHT 10
#define PIECE_SIZE 60

#define BG_COLOR RGB(248, 207, 119)
#define RIVER_COLOR RGB(50, 140, 230)
#define PIECE_COLOR RGB(243, 231, 137)
#define BOX_COLOR RGB(233, 216, 133)
#define GRAY RGB(100, 100, 100)
#define ORANGE RGB(243, 173, 29)
#define DEEP_GREEN RGB(0, 128, 0)
#define PURPLE RGB(123, 31, 162)

const std::string chessboard_txt_path = "./chessboard.txt";
const std::string amount_txt_path = "./amount.txt";
const std::string record_txt_path = "./review_code.txt";
const LPCWSTR chess_ico_path = _T("./chess.ico");