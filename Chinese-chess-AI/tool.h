#pragma once
#include "Headers.h"

namespace tool {

	bool isWin(char who);

	bool isCheck(char who);
	
	extern ExMessage message;
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

	std::string chessboardToFanStr(const CHESS_BOARD& chessboard);

	CHESS_BOARD fanStrToChessboard(const std::string fanStr);

	std::string getTime();

	int getTotalPages(int rows);

	void copy(std::string& text);

	std::string paste();

	std::pair<coordinate, coordinate> getRandomFirstMove();

	bool is_tie(CHESS_BOARD& chess_board);

	Record code_to_record(std::string code, std::string path);
}; 

