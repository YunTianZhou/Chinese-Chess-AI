#include "tool.h"
#include "PublicResource.h"

bool tool::isWin(char who)
{
    CHESS_BOARD& chess_board = PublicResource::Cma->get_checkerboard();
    for (int i = 0; i < chess_board.size(); i++)
    {
        for (int j = 0; j < chess_board[0].size(); j++)
        {
            if (chess_board[i][j] != "sp" and chess_board[i][j][0] != who)
            {
                getTargets({ j,i });

                CMA_RESULT_SET result = PublicResource::Cma->get_result_set();

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
                        PublicResource::Cma->clear_result_set();
                        return false;
                    }

                }
            }
        }
    }
    PublicResource::Cma->clear_result_set();
    return true;
}

bool tool::isCheck(char who)
{
    bool is_call = false;
    CHESS_BOARD& chess_board = PublicResource::Cma->get_checkerboard();
    for (int i = 0; i < chess_board.size(); i++)
    {
        for (int j = 0; j < chess_board[0].size(); j++)
        {
            if (chess_board[i][j][0] == who)
            {
                getTargets({ j,i });

                for (const cma::coordinate& coord : PublicResource::Cma->get_result_set())
                {
                    if (PublicResource::Cma->get_piece_at_coordinate(coord.x, coord.y)[1] == 'l')
                    {
                        is_call = true;
                        break;
                    }
                }
            }
        }
    }
    PublicResource::Cma->clear_result_set();
    return is_call;
}


ExMessage tool::message;
void tool::getMouseMessage(int& x, int& y, bool& leftButton)
{
    peekmessage(&tool::message, EX_MOUSE | EX_KEY);
    x = tool::message.x;
    y = tool::message.y;
    leftButton = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

void tool::remove_check(const coordinate& select)
{
    CHESS_BOARD& chess_board = PublicResource::Cma->get_checkerboard();
    CMA_RESULT_SET result = PublicResource::Cma->get_result_set();
    char color = PublicResource::Cma->get_piece_at_coordinate(select.x, select.y)[0] == 'r' ? 'b' : 'r';

    PublicResource::Cma->screening_destination_customize([&chess_board, &select, &color](const cma::coordinate& coord, CMA_AUTO) {
        std::string record = chess_board[coord.y][coord.x];
        chess_board[coord.y][coord.x] = chess_board[select.y][select.x];
        chess_board[select.y][select.x] = "sp";

        bool is_call = isCheck(color);

        chess_board[select.y][select.x] = chess_board[coord.y][coord.x];
        chess_board[coord.y][coord.x] = record;

        return is_call;

        }, cma::screening_mode::__filter__, result);
    PublicResource::Cma->get_result_set() = result;
}

void tool::getTargets(const coordinate& location)
{
    std::string self = PublicResource::Cma->get_piece_at_coordinate(location.x, location.y);
    char color = self[0];
    PublicResource::Cma->clear_result_set();
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
    PublicResource::Cma->detect_destination_ray({location.x, location.y}, {CMA_BASIC_DIRECTIONS}, att);
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

    if (!PublicResource::Cma->is_out(x - 1, y))
    {
        if (PublicResource::Cma->get_piece_at_coordinate(x - 1, y) == "sp")
        {
            PublicResource::Cma->detect_destination_point({ x,y }, { x - 2, y - 1 }, func);
            PublicResource::Cma->detect_destination_point({ x,y }, { x - 2, y + 1 }, func);
        }
    }
    if (!PublicResource::Cma->is_out(x + 1, y))
    {
        if (PublicResource::Cma->get_piece_at_coordinate(x + 1, y) == "sp")
        {
            PublicResource::Cma->detect_destination_point({ x,y }, { x + 2, y - 1 }, func);
            PublicResource::Cma->detect_destination_point({ x,y }, { x + 2, y + 1 }, func);
        }
    }
    if (!PublicResource::Cma->is_out(x, y - 1))
    {
        if (PublicResource::Cma->get_piece_at_coordinate(x, y - 1) == "sp")
        {
            PublicResource::Cma->detect_destination_point({ x,y }, { x - 1, y - 2 }, func);
            PublicResource::Cma->detect_destination_point({ x,y }, { x + 1, y - 2 }, func);
        }
    }
    if (!PublicResource::Cma->is_out(x, y + 1))
    {
        if (PublicResource::Cma->get_piece_at_coordinate(x, y + 1) == "sp")
        {
            PublicResource::Cma->detect_destination_point({ x,y }, { x - 1, y + 2 }, func);
            PublicResource::Cma->detect_destination_point({ x,y }, { x + 1, y + 2 }, func);
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
    PublicResource::Cma->detect_destination_line({ location.x, location.y }, { CMA_EXTENDED_DIRECTIONS }, att);
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
    PublicResource::Cma->detect_destination_line({ location.x, location.y }, {CMA_EXTENDED_DIRECTIONS}, att);
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
    PublicResource::Cma->detect_destination_line({ location.x, location.y }, { CMA_BASIC_DIRECTIONS }, att);
    std::vector<std::vector<std::string>>& chess_board = PublicResource::Cma->get_checkerboard();
    int my = who == 'r' ? -1 : 1;
    for (int y = location.y + my; y >= 0 and y < chess_board.size(); y += my)
    {
        if (chess_board[y][location.x] != "sp")
        {
            if (chess_board[y][location.x][1] == 'l')
            {
                PublicResource::Cma->get_result_set().insert({ location.x , y });
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
    PublicResource::Cma->detect_destination_ray({ location.x, location.y }, { CMA_BASIC_DIRECTIONS }, att);
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
    PublicResource::Cma->detect_destination_line({ location.x, location.y }, dirs, att);

}

std::string tool::chessboardToFanStr(const CHESS_BOARD& chessboard)
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

CHESS_BOARD tool::fanStrToChessboard(const std::string fanStr)
{
    CHESS_BOARD chessBoard(GRID_HEIGHT, std::vector<std::string>(GRID_WIDTH, "sp"));
    int str_ptr = 0;
    int chessboard_ptr = 0;
    size_t length = fanStr.size();
    while (str_ptr < length)
    {
        int cnt = 0;
        while (str_ptr < length && fanStr[str_ptr] >= '0' && fanStr[str_ptr] <= '9')
        {
            cnt *= 10;
            cnt += fanStr[str_ptr] - '0';
            str_ptr++;
        }
        chessboard_ptr += cnt;
        if (str_ptr < length)
        {
            chessBoard[chessboard_ptr / GRID_WIDTH][chessboard_ptr % GRID_WIDTH] = fanStr.substr(str_ptr, 2);
            chessboard_ptr++;
            str_ptr += 2;
        }
    }
    return chessBoard;
}

std::string tool::getTime()
{
    std::time_t now = std::time(nullptr);
    std::tm localTimeStruct;
    localtime_s(&localTimeStruct, &now);

    std::stringstream ss;
    ss << std::put_time(&localTimeStruct, "%Y-%m-%d");
    std::string currentDate = ss.str();
    return currentDate;
}

int tool::getTotalPages(int rows)
{
    return (int)std::ceil((float)rows / (float)record_page_limit) - 1;
}

void tool::copy(std::string& text)
{
    if (!OpenClipboard(nullptr)) {
        std::cerr << "Failed to open clipboard." << std::endl;
        return;
    }

    EmptyClipboard();

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (text.length() + 1) * sizeof(char));
    if (hMem == nullptr) {
        CloseClipboard();
        std::cerr << "Failed to allocate memory." << std::endl;
        return;
    }

    char* pMem = static_cast<char*>(GlobalLock(hMem));
    if (pMem == nullptr) {
        GlobalFree(hMem);
        CloseClipboard();
        std::cerr << "Failed to lock memory." << std::endl;
        return;
    }

    strcpy_s(pMem, text.length() + 1, text.c_str());

    GlobalUnlock(hMem);

    SetClipboardData(CF_TEXT, hMem);

    CloseClipboard();
}

std::string tool::paste()
{
    std::string clipboardText;

    if (!OpenClipboard(nullptr)) {
        std::cerr << "Failed to open clipboard." << std::endl;
        return clipboardText;
    }

    HANDLE hClipboardData = GetClipboardData(CF_TEXT);
    if (hClipboardData == nullptr) {
        std::cerr << "Failed to get clipboard data." << std::endl;
        CloseClipboard();
        return clipboardText;
    }

    char* pszText = static_cast<char*>(GlobalLock(hClipboardData));
    if (pszText == nullptr) {
        std::cerr << "Failed to lock clipboard data." << std::endl;
        CloseClipboard();
        return clipboardText;
    }

    clipboardText = pszText;

    GlobalUnlock(hClipboardData);
    CloseClipboard();

    return clipboardText;
}

std::pair<coordinate, coordinate> tool::getRandomFirstMove()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, (int)PublicResource::first_move.size() - 1);
    return PublicResource::first_move[dist(gen)];
}

bool tool::is_tie(CHESS_BOARD& chess_board)
{
    int cnt_sp = 0;
    int cnt_rp = 0;
    int cnt_bp = 0;
    bool tie = true;

    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++) 
        {
            tie &= chess_board[i][j] == "sp" || chess_board[i][j][1] == 'l' ||
                chess_board[i][j][1] == 's' || chess_board[i][j][1] == 'x' || 
                (i == 0 && chess_board[i][j] == "rb" || i == GRID_HEIGHT - 1 && chess_board[i][j] == "bb");
            cnt_sp += chess_board[i][j] == "sp";
            cnt_rp += chess_board[i][j] == "rp";
            cnt_bp += chess_board[i][j] == "bp";
        }
    }

    int total = GRID_HEIGHT * GRID_WIDTH;
    return tie || (cnt_sp == total - 3 && (cnt_rp || cnt_bp)) || (cnt_sp == total - 4 && cnt_rp && cnt_bp);
}

Record tool::code_to_record(std::string code, std::string path)
{
    int count = 0;
    for (char ch : code) {
        if (ch == '/') {
            count++;
        }
    }
    if (count != 7) throw std::invalid_argument("/ != 7");

    std::stringstream ss(code);
    std::string token;
    std::vector<std::string> split_strings;
    for (int i = 0; i < 5; i++) {
        std::getline(ss, token, '/');
        split_strings.push_back(token);
    }
    std::vector<bool> tickets;
    if (split_strings[4].size() != 4) throw std::invalid_argument("size != 4");
    for (int i = 0; i < split_strings[4].size(); i++)
    {
        tickets.push_back(split_strings[4][i] == '1');
    }
    return { path, split_strings[0], split_strings[1][0],
        PublicResource::code_to_explaintion.at(std::stoi(split_strings[2])),
        split_strings[3] == "1", tickets };
}

