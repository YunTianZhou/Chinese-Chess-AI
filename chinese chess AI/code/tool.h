#pragma once
#include "Headers.h"

namespace tool {
	extern std::unordered_map<std::string, const wchar_t> translater;
	extern std::unordered_map<char, int> piece_power;
	extern std::unordered_map<char, int> piece_activity;
	extern const int cross_river_power;
	extern const int first_move_j_power;

	extern cma::ChessMovesAnalyze<std::string>* Cma;

	bool isWin(char who/*红 or 黑*/);
	bool isCheck(char who/*红 or 黑*/);

	CHESS_BOARD readFileGetChessBoard(const std::string& path);

	int readFileGetAIAmount(const std::string& path);

	void writeFileSaveRecord(const std::string& path, const std::string& record);

	std::string readFileGetRecord(const std::string& path);

	void getMouseMessage(int& x, int& y, bool& leftButton);

	void remove_check(const coordinate& select);

	void getTargets(const coordinate& location);

	void getTargets_j(const coordinate& location, char who);
						
	void getTargets_m(const coordinate& location, char who);
						
	void getTargets_x(const coordinate& location, char who);
						
	void getTargets_s(const coordinate& location, char who);
						
	void getTargets_l(const coordinate& location, char who);
						
	void getTargets_p(const coordinate& location, char who);
											    
	void getTargets_b(const coordinate& location, char who);

	std::string chessboardToFanStr(CHESS_BOARD& chessboard);

};

