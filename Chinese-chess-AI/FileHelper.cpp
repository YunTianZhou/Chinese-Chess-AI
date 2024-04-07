#include "FileHelper.h"
#include "tool.h"
#include "PublicResource.h"

void FileHelper::readFileGetChessBoard(CHESS_BOARD& chessBoard)
{
    std::ifstream ifs(chessboard_txt_path);
    std::string fanStr;
    std::getline(ifs, fanStr);
    chessBoard = tool::fanStrToChessboard(fanStr);
}

void FileHelper::readFileGetOriginalChessBoard(CHESS_BOARD& chessBoard)
{
    std::ifstream ifs(original_chessBoard_txt_path);
    std::string fanStr;
    std::getline(ifs, fanStr);
    chessBoard = tool::fanStrToChessboard(fanStr);
}

size_t FileHelper::readFileGetAIAmount()
{
    std::ifstream ifs(amount_txt_path);
    std::string line;
    std::getline(ifs, line);
    return std::stol(line);
}

Record FileHelper::writeFileSaveRecord(std::string path, char finaly, std::wstring way,
    const std::string& record, const CHESS_BOARD& statr, const CHESS_BOARD& end)
{
    std::ofstream ofs(path);
    std::string time = tool::getTime();
    ofs << time << "/";
    ofs << finaly << "/";
    std::wstring str = way;
    ofs << PublicResource::explaintion_to_code.at(way) << "/";
    ofs << PublicResource::current_go_first << "/0000/";
    Record res(path, time, finaly, way, PublicResource::current_go_first, {0, 0, 0, 0});
    std::string original_fanStr = tool::chessboardToFanStr(PublicResource::current_original_chessBoard);
    std::string fanStr = tool::chessboardToFanStr(statr);
    ofs << (fanStr == original_fanStr ? "original" : fanStr) << "/";
    fanStr = tool::chessboardToFanStr(end);
    ofs << (fanStr == original_fanStr ? "original" : fanStr) << "/";
    ofs << record << std::endl;
    return res;
}

void FileHelper::readFileGetRecordInfo(std::string& record, CHESS_BOARD& start, CHESS_BOARD& end, bool& go_first)
{
    std::ifstream ifs(curr_review_code_txt_path);
    std::string line;
    std::getline(ifs, line);
    std::vector<std::string> splittedString;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '/')) {
        splittedString.push_back(token);
    }
    go_first = splittedString[3] == "1";
    start = splittedString[5] == "original" ? PublicResource::current_chessBoard :
        tool::fanStrToChessboard(splittedString[5]);
    end = splittedString[6] == "original" ? PublicResource::current_chessBoard :
        tool::fanStrToChessboard(splittedString[6]);
    record = splittedString[7];
}

bool FileHelper::readFileGetGoFirst()
{
    std::ifstream ifs(go_first_txt_path);
    std::string who;
    std::getline(ifs, who);
    return who == "player";
}

bool FileHelper::readFileGetAutomaticallyAdd()
{
    std::ifstream ifs(automatically_add_txt_path);
    std::string who;
    std::getline(ifs, who);
    return who == "1";
}

int FileHelper::readFileGetId(const std::string& folderPath) {
    int Max = 0;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry)) {
            std::string name = entry.path().filename().string();
            if (name == "control.txt" || name == "join.txt") continue;
            Max = max(Max, std::stoi(name));
        }
    }
    return Max;
}

void FileHelper::copyFile(const std::string& source_filename, const std::string& destination_filename)
{
    std::ifstream source_file(source_filename);
    std::ofstream destination_file(destination_filename);

    std::string line;
    while (std::getline(source_file, line)) {
        destination_file << line << std::endl;
    }

    source_file.close();
    destination_file.close();
}

void FileHelper::getAllRecordsInFoder(std::string path, std::vector<Record>& records)
{
    std::ifstream ifs(path + "/control.txt");
    std::string line;
    records.clear();
    while (std::getline(ifs, line)) {
        records.push_back(getRecordInFile(path + "/" + line));
    }
    ifs.close();
}

void FileHelper::getLinkedRecord(std::vector<Record>& recent_list, std::vector<Record>& my_list, std::vector<std::pair<Record, Record>>& linked_record)
{
    // clear the record 
    linked_record.clear();

    // Open the file join.txt
    std::string join_filepath = recent_record_list_path + "/join.txt";
    std::ifstream join_ifs(join_filepath);

    // Get each line in file
    std::string line;
    while (std::getline(join_ifs, line))
    {
        // Split each line by '/', get the to linked-files
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (std::getline(ss, token, '/')) {
            tokens.push_back(token);
        }

        // Get the total path of the linked-files
        std::string recent_linked_filename_total_path = recent_record_list_path + "/" + tokens[0];
        std::string my_linked_filename_total_path = my_record_list_path + "/" + tokens[1];

        // Use the total path to find the two linked Record obj
        Record* recent_linked_obj = nullptr;
        Record* my_linked_obj = nullptr;

        // Find recent linked Record obj
        for (int i = 0; i < recent_list.size(); i++)
        {
            if (recent_list[i].path == recent_linked_filename_total_path)
            {
                recent_linked_obj = &recent_list[i]; 
                break;
            }
        }

        // Find my linked Record obj
        for (int i = 0; i < my_list.size(); i++)
        {
            if (my_list[i].path == my_linked_filename_total_path)
            {
                my_linked_obj = &my_list[i];
                break;
            }
        }

        // Push the linked-obj into the record
        if (recent_linked_obj && my_linked_obj)
            linked_record.push_back({ *recent_linked_obj, *my_linked_obj });
    }
}

Record FileHelper::getRecordInFile(std::string path)
{
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        if (std::getline(file, line)) {
            file.close();
            return tool::code_to_record(line, path);
        }
    }
    return Record("", "error", '\0', _T("unlegal-filepath"), false, {0, 0, 0, 0});
}

void FileHelper::insertContentAtBeginning(const std::string& filePath, const std::string& contentToInsert)
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << contentToInsert << "\n";

    std::string line;
    while (std::getline(inFile, line)) {
        buffer << line << "\n";
    }
    inFile.close();

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }
    outFile << buffer.str();
    outFile.close();
}

void FileHelper::removeLineInFile(std::string path, const std::string& target)
{
    // Open the file
    std::ifstream inFile(path);

    // Read the file
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line)) {
        // If the line is not the target line, save it
        if (line != target) {
            lines.push_back(line);
        }
    }
    inFile.close();

    // Write the info back to the file
    std::ofstream outFile(path);
    for (const std::string& l : lines) {
        outFile << l << std::endl;
    }
    outFile.close();
}
