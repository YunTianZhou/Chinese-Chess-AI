#pragma once
#include "Headers.h"

namespace FileHelper
{
	void readFileGetChessBoard(CHESS_BOARD& chessBoard);
	void readFileGetOriginalChessBoard(CHESS_BOARD& chessBoard);
	size_t readFileGetAIAmount();
	Record writeFileSaveRecord(std::string path, char finaly, std::wstring way,
		const std::string& record, const CHESS_BOARD& statr, const CHESS_BOARD& end);
	void readFileGetRecordInfo(std::string& record, CHESS_BOARD& start, CHESS_BOARD& end, bool& go_first);
	bool readFileGetGoFirst();
	bool readFileGetAutomaticallyAdd();
	int readFileGetId(const std::string& folderPath);
	void copyFile(const std::string& source_filename, const std::string& destination_filename);
	void getAllRecordsInFoder(std::string path, std::vector<Record>& records);
	void getLinkedRecord(std::vector<Record>& recent_list, std::vector<Record>& my_list, std::vector<std::pair<Record, Record>>& linked_record);
	Record getRecordInFile(std::string path);
	void insertContentAtBeginning(const std::string& filePath, const std::string& contentToInsert);
	void removeLineInFile(std::string path, const std::string& target);
}