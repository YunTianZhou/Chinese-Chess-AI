#pragma once
#include "Headers.h"
class button
{
private:
	int Left;
	int Right;
	int Top;
	int Botton;
public:
	button(int Left, int Top, int Right, int Botton);
	bool is_click(int x, int y, bool leftButton);
};

