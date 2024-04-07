#include "Ai.h"
#include "tool.h"
#include "PublicResource.h"

Ai::Ai() : chess_board(nullptr),
last_move_begin_coordinate(nullptr),
last_move_end_coordinate(nullptr),
keepCheckTimes(0),
keepCheckRecord(nullptr)
{
    
}

Ai::~Ai()
{
    
}

void Ai::Bind(CHESS_BOARD* chess_board,
    coordinate* last_move_begin_coordinate,
    coordinate* last_move_end_coordinate,
    int* keepCheckTimes,
    std::unordered_set<std::string>* keepCheckRecord)
{
    this->chess_board = chess_board;
    this->last_move_begin_coordinate = last_move_begin_coordinate;
    this->last_move_end_coordinate = last_move_end_coordinate;
    this->keepCheckTimes = keepCheckTimes;
    this->keepCheckRecord = keepCheckRecord;
}

bool Ai::go(int first) {
    if (!first && !PublicResource::current_go_first && PublicResource::current_original_chessBoard == PublicResource::current_chessBoard)
    {
        std::pair<coordinate, coordinate> first_move = tool::getRandomFirstMove();
        last_move_begin_coordinate->set(first_move.first.x, first_move.first.y);
        last_move_end_coordinate->set(first_move.second.x, first_move.second.y);
        return false;
    }

    int target_num = 0;
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if ((*chess_board)[i][j] != "sp") {
                tool::getTargets({ j,i });
                target_num += (int)PublicResource::Cma->get_result_set().size();
            }
        }
    }
    int depth = (int)((int)log(PublicResource::current_amount) / log(target_num / 2));
    int respons = MiniMax(depth, true, true, -1000000000, 1000000000);
    PublicResource::Cma->clear_result_set();
    return respons == -PublicResource::piece_power.at('l') * ((depth - 1) * 100)
        || respons == INT_MIN;
}

int Ai::MiniMax(int depth, bool first, bool blackMove, int alpha, int beta) {
    if (!depth) {
        return searchAllCaptures(blackMove, alpha, beta);
    }
    CHESS_BOARD& chessBoard = (*chess_board);
    if (*keepCheckTimes >= 2)
    {
        const std::string& currFanStr = tool::chessboardToFanStr(chessBoard);
        if (keepCheckRecord->find(currFanStr) != keepCheckRecord->end())
        {
            return INT_MIN;
        }
    }
    char self = blackMove ? 'b' : 'r';
    coordinate start;
    coordinate end;
    std::vector<std::pair<coordinate, coordinate>> Moves;
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (chessBoard[i][j][0] == self) {
                tool::getTargets({ j,i });
                CMA_RESULT_SET& result = PublicResource::Cma->get_result_set();
                for (const cma::coordinate& coord : result) {
                    Moves.push_back({ { j,i }, { coord.x, coord.y } });
                }
            }
        }
    }

    // sort the Moves can make more likely to prom
    std::sort(Moves.begin(), Moves.end(), [this](
        const std::pair<coordinate, coordinate>& first,
        const std::pair<coordinate, coordinate>& second)
        {
            return scoring(first.second.x, first.second.y) >
                scoring(second.second.x, second.second.y);
        }
    );

    for (int i = 0; i < Moves.size(); i++)
    {
        int startx = Moves[i].first.x;
        int starty = Moves[i].first.y;
        int endx = Moves[i].second.x;
        int endy = Moves[i].second.y;
        if (chessBoard[endy][endx][1] == 'l') {
            return PublicResource::piece_power.at('l') * (depth * 100);
        }
        std::string record = chessBoard[endy][endx];
        chessBoard[endy][endx] = chessBoard[starty][startx];
        chessBoard[starty][startx] = "sp";
        int score = -MiniMax(depth - 1, false, !blackMove, -beta, -alpha);
        chessBoard[starty][startx] = chessBoard[endy][endx];
        chessBoard[endy][endx] = record;
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            start.set(startx, starty);
            end.set(endx, endy);
        }
    }

    if (first) {
        last_move_begin_coordinate->set(start.x, start.y);
        last_move_end_coordinate->set(end.x, end.y);
    }
    return alpha;
}

int Ai::searchAllCaptures(bool blackMove, int alpha, int beta)
{
    int currentScore = scoring(blackMove);
    alpha = max(alpha, currentScore);

    if (alpha >= beta)
    {
        return beta;
    }

    CHESS_BOARD& chessBoard = (*chess_board);
    char self = blackMove ? 'b' : 'r';
    std::vector<std::pair<coordinate, coordinate>> Moves;
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (chessBoard[i][j][0] == self) {
                tool::getTargets({ j,i });
                CMA_RESULT_SET& result = PublicResource::Cma->get_result_set();
                for (const cma::coordinate& coord : result) {
                    if (chessBoard[coord.y][coord.x] != "sp")
                    {
                        Moves.push_back({ { j,i }, { coord.x, coord.y } });
                    }
                }
            }
        }
    }

    // sort the Moves can make more likely to prom
    std::sort(Moves.begin(), Moves.end(), [this](
        const std::pair<coordinate, coordinate>& first,
        const std::pair<coordinate, coordinate>& second)
        {
            return scoring(first.second.x, first.second.y) >
                scoring(second.second.x, second.second.y);
        }
    );

    for (int i = 0; i < Moves.size(); i++)
    {
        int startx = Moves[i].first.x;
        int starty = Moves[i].first.y;
        int endx = Moves[i].second.x;
        int endy = Moves[i].second.y;
        if (chessBoard[endy][endx][1] == 'l') {
            return PublicResource::piece_power.at('l') * 100;
        }
        std::string record = chessBoard[endy][endx];
        chessBoard[endy][endx] = chessBoard[starty][startx];
        chessBoard[starty][startx] = "sp";
        int score = -searchAllCaptures(!blackMove, -beta, -alpha);
        chessBoard[starty][startx] = chessBoard[endy][endx];
        chessBoard[endy][endx] = record;
        if (score >= beta) {
            return beta;
        }
        alpha = max(alpha, score);
    }
    return alpha;
}

int Ai::scoring(bool blackMove)
{
    CHESS_BOARD& chessBoard = (*chess_board);
    int total = 0;
    char self = blackMove ? 'b' : 'r';
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++) 
        {
            if (chessBoard[i][j] != "sp")
            {
                int score = scoring(j, i);
                if (chessBoard[i][j][0] == self)
                {
                    total += score;
                }
                else
                {
                    total -= score;
                }
            }
        }
    }
    return total;
}

int Ai::scoring(int x, int y)
{
    CHESS_BOARD& chessBoard = (*chess_board);
    int score = 0;
    char color = chessBoard[y][x][0];
    char self = chessBoard[y][x][1];
    bool cross_river = (color == 'r') ? (y < 5) : (y > 4);
    if (chessBoard[y][x] == "sp") return 0;
    else if (self == 'm' || self == 'j')
    {
        tool::getTargets({ x,y });
        CMA_RESULT_SET& result = PublicResource::Cma->get_result_set();
        score += (int)result.size() * PublicResource::piece_activity.at(self);
    }
    score += PublicResource::piece_power.at(self);
    if (color == 'r')
    {
        score += PublicResource::piece_position_score.at(self)
            [GRID_HEIGHT - y - 1][GRID_WIDTH - x - 1];
    }
    else
    {
        score += PublicResource::piece_position_score.at(self)[y][x];
    }
    if (chessBoard[y][x][1] == 'p' && x == 4)
    {
        if (color == 'b') y++; else y--;
        while (0 <= y && y < GRID_HEIGHT)
        {
            if (chessBoard[y][x] != "sp")
            {
                score += (chessBoard[y][x][1] == 'l' && chessBoard[y][x][0] != color) * 40;
                break;
            }
            if (color == 'b') y++; else y--;
        }
    }
    return score;
}




