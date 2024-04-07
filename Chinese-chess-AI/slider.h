#pragma once
#include "button.h"

class slider : public Button
{
private:
	int button_height;
	int slider_x;
	int slider_length;
	size_t* var;
	const std::vector<size_t>* values;
	bool sliding;
	float tile;
	int position;
public:
	slider(int button_y, int button_width, int button_height,
		int slider_x, int slider_length, size_t* var, const std::vector<size_t>* values);
	bool listen();
};

