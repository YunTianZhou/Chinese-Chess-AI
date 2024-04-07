#pragma once
#include <string>
#include <vector>

class Record
{
public:
	std::string path;
	std::string date;
	char result;
	std::wstring reason;
	bool go_first;
	std::vector<bool> tickets;

	Record(std::string path, std::string date, char result, std::wstring reason, bool go_first, std::vector<bool> tickets)
	{
		this->path = path;
		this->date = date;
		this->result = result;
		this->reason = reason;
		this->go_first = go_first;
		this->tickets = tickets;
	}

	bool operator==(const Record& other) const
	{
		return path == other.path;
	}
};

