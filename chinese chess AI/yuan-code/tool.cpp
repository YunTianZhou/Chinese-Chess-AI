#include "tool.h"

std::unordered_map<std::string, const wchar_t> tool::translater = {
    {"rj", L'俥'},
    {"bj", L'車'},

    {"rm", L'傌'},
    {"bm", L'馬'},

    {"rx", L'相'},
    {"bx", L'象'},

    {"rs", L'仕'},
    {"bs", L'士'},

    {"rl", L'帥'},
    {"bl", L'将'},

    {"rp", L'炮'},
    {"bp", L'炮'},

    {"rb", L'兵'},
    {"bb", L'卒'},
};

std::unordered_map<char, int> tool::piece_power = {
    {'j', 180},
    {'m', 60},
    {'x', 40},
    {'s', 40},
    {'l', 10000},
    {'p', 100}
};

std::unordered_map<char, int> tool::piece_activity =
{
    {'j', 1},
    {'m', 5}
};

const int tool::cross_river_power = 10;

const int tool::first_move_j_power = 5;

cma::ChessMovesAnalyze<std::string>* tool::Cma = nullptr;

bool tool::isWin(char who)
{
    CHESS_BOARD& chess_board = Cma->get_checkerboard();
    for (int i = 0; i < chess_board.size(); i++)
    {
        for (int j = 0; j < chess_board[0].size(); j++)
        {
            if (chess_board[i][j] != "sp" and chess_board[i][j][0] != who)
            {
                getTargets({ j,i });

                CMA_RESULT_SET result = Cma->get_result_set();

                for (const cma::coordinate& coord : result)
                {
                    std::string record = chess_board[coord.y][coord.x];
                    chess_board[coord.y][coord.x] = chess_board[i][j];
                    chess_board[i][j] = "sp";

                    bool is_call = isCheck(who);

                    chess_board[i][j] = chess_board[coord.y][coord.x];
                    chess_board[coord.y][coord.x] = record;

                    if (!is_call)
                    {
                        Cma->clear_result_set();
                        return false;
                    }

                }
            }
        }
    }
    Cma->clear_result_set();
    return true;
}

bool tool::isCheck(char who)
{
    bool is_call = false;
    CHESS_BOARD& chess_board = Cma->get_checkerboard();
    for (int i = 0; i < chess_board.size(); i++)
    {
        for (int j = 0; j < chess_board[0].size(); j++)
        {
            if (chess_board[i][j][0] == who)
            {
                getTargets({ j,i });

                for (const cma::coordinate& coord : Cma->get_result_set())
                {
                    if (Cma->get_piece_at_coordinate(coord.x, coord.y)[1] == 'l')
                    {
                        is_call = true;
                        break;
                    }
                }
            }
        }
    }
    Cma->clear_result_set();
    return is_call;
}

CHESS_BOARD tool::readFileGetChessBoard(const std::string& path)
{
    std::ifstream ifs(path);

    std::string line;
    CHESS_BOARD result;

    while (std::getline(ifs, line)) {
        std::istringstream ss(line);
        std::string token = "";
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        result.push_back(tokens);
    }

    return result;
}

int tool::readFileGetAIAmount(const std::string& path)
{
    std::ifstream ifs(path);
    std::string line;
    std::getline(ifs, line);
    return std::stoi(line);
}

void tool::writeFileSaveRecord(const std::string& path, const std::string& record)
{
    std::ofstream ofs(path);
    ofs << record;
}

std::string tool::readFileGetRecord(const std::string& path)
{
    std::ifstream ifs(path);
    std::string line;
    std::getline(ifs, line);
    return line;
}

void tool::getMouseMessage(int& x, int& y, bool& leftButton)
{
    ExMessage message = getmessage(EX_MOUSE | EX_KEY);
    x = message.x;
    y = message.y;  
    leftButton = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
}

void tool::remove_check(const coordinate& select)
{
    CHESS_BOARD& chess_board = Cma->get_checkerboard();
    CMA_RESULT_SET result = Cma->get_result_set();

    Cma->screening_destination_customize([&chess_board, &select](const cma::coordinate& coord, CMA_AUTO) {
        std::string record = chess_board[coord.y][coord.x];
        chess_board[coord.y][coord.x] = chess_board[select.y][select.x];
        chess_board[select.y][select.x] = "sp";

        bool is_call = isCheck('b');

        chess_board[select.y][select.x] = chess_board[coord.y][coord.x];
        chess_board[coord.y][coord.x] = record;

        return is_call;

        }, cma::screening_mode::__filter__, result);
    Cma->get_result_set() = result;
}

void tool::getTargets(const coordinate& location)
{
    std::string self = Cma->get_piece_at_coordinate(location.x, location.y);
    char color = self[0];
    Cma->clear_result_set();
    switch (self[1])
    {
    case 'j':
        getTargets_j(location, color);
        break;
    case 'm':
        getTargets_m(location, color);
        break;
    case 'x':
        getTargets_x(location, color);
        break;
    case 's':
        getTargets_s(location, color);
        break;
    case 'l':
        getTargets_l(location, color);
        break;
    case 'p':
        getTargets_p(location, color);
        break;
    case 'b':
        getTargets_b(location, color);
        break;
    }
}

void tool::getTargets_j(const coordinate& location, char who)
{
    cma::ChessMovesAnalyze<std::string>::att_ray att = {
            true, 0, [](CMA_AUTO, CMA_AUTO, const std::string& curr, CMA_AUTO) {return curr != "sp"; }, CMA_NULL,
            [who](CMA_AUTO, CMA_AUTO, const std::string& curr, CMA_AUTO, CMA_AUTO) {return curr[0] != who; }
    };
    Cma->detect_destination_ray({location.x, location.y}, {CMA_BASIC_DIRECTIONS}, att);
}

void tool::getTargets_m(const coordinate& location, char who)
{
    int x = location.x;
    int y = location.y;
    std::function<bool(const cma::coordinate&,
        const cma::coordinate&, const std::string&,
        const std::string&, const cma::direction)> func = 
        [&who](CMA_AUTO, CMA_AUTO, const std::string& cuur, CMA_AUTO, CMA_AUTO) {
            return cuur[0] != who;
        };

    if (!Cma->is_out(x - 1, y))
    {
        if (Cma->get_piece_at_coordinate(x - 1, y) == "sp")
        {
            Cma->detect_destination_point({ x,y }, { x - 2, y - 1 }, func);
            Cma->detect_destination_point({ x,y }, { x - 2, y + 1 }, func);
        }
    }
    if (!Cma->is_out(x + 1, y))
    {
        if (Cma->get_piece_at_coordinate(x + 1, y) == "sp")
        {
            Cma->detect_destination_point({ x,y }, { x + 2, y - 1 }, func);
            Cma->detect_destination_point({ x,y }, { x + 2, y + 1 }, func);
        }
    }
    if (!Cma->is_out(x, y - 1))
    {
        if (Cma->get_piece_at_coordinate(x, y - 1) == "sp")
        {
            Cma->detect_destination_point({ x,y }, { x - 1, y - 2 }, func);
            Cma->detect_destination_point({ x,y }, { x + 1, y - 2 }, func);
        }
    }
    if (!Cma->is_out(x, y + 1))
    {
        if (Cma->get_piece_at_coordinate(x, y + 1) == "sp")
        {
            Cma->detect_destination_point({ x,y }, { x - 1, y + 2 }, func);
            Cma->detect_destination_point({ x,y }, { x + 1, y + 2 }, func);
        }
    }
}

void tool::getTargets_x(const coordinate& location, char who)
{
    bool isCross = false;
    cma::ChessMovesAnalyze<std::string>::att_line att = {
        2, 
        [&isCross, &who](const cma::coordinate& coord, 
        CMA_AUTO,const std::string& curr, CMA_AUTO, size_t length) 
        {
            if (length == 1) 
            {
                isCross = curr == "sp";
                return false;
            }
            bool cross_river = (who == 'r') ? (coord.y < 5) : (coord.y > 4);
            return !cross_river and isCross and curr[0] != who;
        }
    };
    Cma->detect_destination_line({ location.x, location.y }, { CMA_EXTENDED_DIRECTIONS }, att);
}

void tool::getTargets_s(const coordinate& location, char who)
{
    cma::ChessMovesAnalyze<std::string>::att_line att = {
        1,
        [&who](const cma::coordinate& coord,
        CMA_AUTO,const std::string& curr, CMA_AUTO, CMA_AUTO)
        {
            bool is_not_out = coord.x > 2 and coord.x < 6 and (coord.y < 3 or coord.y > 6);
            return curr[0] != who and is_not_out;
        }
    };
    Cma->detect_destination_line({ location.x, location.y }, {CMA_EXTENDED_DIRECTIONS}, att);
}

void tool::getTargets_l(const coordinate& location, char who)
{
    cma::ChessMovesAnalyze<std::string>::att_line att = {
    1,
    [&who](const cma::coordinate& coord,
    CMA_AUTO,const std::string& curr, CMA_AUTO, CMA_AUTO)
    {
        bool is_not_out = coord.x > 2 and coord.x < 6 and (coord.y < 3 or coord.y > 6);
        return curr[0] != who and is_not_out;
    }
    };
    Cma->detect_destination_line({ location.x, location.y }, { CMA_BASIC_DIRECTIONS }, att);
    std::vector<std::vector<std::string>>& chess_board = Cma->get_checkerboard();
    int my = who == 'r' ? -1 : 1;
    for (int y = location.y + my; y >= 0 and y < chess_board.size(); y += my)
    {
        if (chess_board[y][location.x] != "sp")
        {
            if (chess_board[y][location.x][1] == 'l')
            {
                Cma->get_result_set().insert({ location.x , y });
            }
            break;
        }
    }
}

void tool::getTargets_p(const coordinate& location, char who)
{
    cma::ChessMovesAnalyze<std::string>::att_ray att = {
         true, 1, [](CMA_AUTO, CMA_AUTO, const std::string& curr, CMA_AUTO) {return curr != "sp"; }, CMA_NULL, CMA_NULL,
         [&who](CMA_AUTO, CMA_AUTO, const std::string& curr, CMA_AUTO, const bool iscross, CMA_AUTO, CMA_AUTO record) {
            if (iscross)
                return record.size() == 1 and curr[0] != who;
            return record.size() == 0;
        }
    };
    Cma->detect_destination_ray({ location.x, location.y }, { CMA_BASIC_DIRECTIONS }, att);
}

void tool::getTargets_b(const coordinate& location, char who)
{
    std::set<cma::direction> dirs = { (who == 'r' ? cma::direction::__up__ : cma::direction::__down__)};
    bool cross_river = (who == 'r') ? (location.y < 5) : (location.y > 4);
    if (cross_river)
    {
        dirs.insert(cma::direction::__left__);
        dirs.insert(cma::direction::__right__);
    }
    cma::ChessMovesAnalyze<std::string>::att_line att = {
        1,
        [&who](const cma::coordinate& coord,
        CMA_AUTO,const std::string& curr, CMA_AUTO, CMA_AUTO)
        {
            return curr[0] != who;
        }
    };
    Cma->detect_destination_line({ location.x, location.y }, dirs, att);

}

std::string tool::chessboardToFanStr(CHESS_BOARD& chessboard)
{
    std::string fanStr;
    int sp_cnt = 0;
    for (const auto& row : chessboard) {
        for (const auto& piece : row) {
            if (piece != "sp")
            {
                if (sp_cnt)
                {
                    fanStr += std::to_string(sp_cnt);
                    sp_cnt = 0;
                }
                fanStr += piece;
            }
            else
            {
                sp_cnt++;
            }
        }
    }
    if (sp_cnt)
    {
        fanStr += std::to_string(sp_cnt);
        sp_cnt = 0;
    }
    return fanStr;
}

