#pragma once
#include "Game.h"
class Interface
{
private:
	Renderer* renderer;
	Game* game;
	std::vector<Record> recent_record_list;
	std::vector<Record> my_record_list;
	std::vector<Record> load_record_list;
	std::vector<std::pair<Record, Record>> linked_record;
public:
	Interface();
	~Interface();
private:
	void main_interface();

	void manage_interface();
	void manage_interface_check_record(Record& record);
	void manage_interface_join_record(Record& record);
	void manage_interface_share_record(Record& record);
	void manage_interface_delete_record(Record& record, std::string foder, size_t id, std::vector<Record>& records);
	void manage_interface_edit_tickets(Record& record);
	void manage_interface_check_tickets(Record& record);

	void settings_interface();
	void settings_interface_init_piece_num(std::vector<size_t>& piece_num);;
	bool settings_interface_reminder();

	void load_interface();
};

