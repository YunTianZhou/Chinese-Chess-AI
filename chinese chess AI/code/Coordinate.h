#pragma once
class coordinate
{
public:
	coordinate() : x(-1), y(-1) {}

	coordinate(int x, int y) : x(x), y(y) {}

	coordinate(const std::string& code) { set(code); }

	void set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void set(const std::string& code)
	{
		x = code[0] - '0';
		y = code[1] - '0';
	}

	void set(const coordinate& other)
	{
		x = other.x;
		y = other.y;
	}

	bool isEmpty()
	{
		return this->x < 0 or this->y < 0;
	}

	std::string to_string()
	{
		return std::to_string(x) + std::to_string(y);
	}

	int x;
	int y;
};



