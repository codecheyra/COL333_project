#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"
#include <unordered_map>

using namespace std;

int evaluate(const Board& b, PlayerColor color);
int minValue(int depth, int alpha, int beta, const Board& b, PlayerColor color);
int maxValue(int depth, int alpha, int beta, const Board& b, PlayerColor color);
U16 alphaBetaPrune(int depth, int alpha, int beta,const Board& b, bool maximisingPlayer, PlayerColor color);

int minValue(int depth, int alpha, int beta,const Board& b, PlayerColor color){
    cout<<"min, depth = "<<depth<<" color = "<<" alpha = "<<alpha<<" beta = "<<beta<<endl;
    Board newBoard = Board(b.data);

    // if (depth == 0){
    //     return evaluate(newBoard, color);
    // }
    int minEval = INFINITY;
    int eval;
    unordered_set <U16> legalMoves = b.get_legal_moves();
    for (const auto move : legalMoves){
        Board newBoard2 = Board(b.data);
        cout<<"move :"<<move_to_str(move)<<endl;
        newBoard2.do_move_(move);
        // if (color == WHITE){
        //     // eval = maxValue(depth-1, alpha, beta, newBoard2, BLACK);
        //     eval = alphaBetaPrune(depth-1, alpha, beta, newBoard2, true, BLACK);
        // }else{
        //     eval = alphaBetaPrune(depth-1, alpha, beta, newBoard2, true, WHITE);
        // }
        eval = alphaBetaPrune(depth-1, alpha, beta, newBoard2, true, color);
        
        
        minEval = min(minEval, eval);
        beta = min(beta, minEval);
        cout<<"after one move min, depth = "<<depth<<" alpha = "<<alpha<<" beta = "<<beta<<" eval = "<<eval<<" minEval = "<<minEval<<endl;
        if (beta <= alpha){
            break;
        }
    
    }
    return minEval;
}

int maxValue(int depth,int alpha, int beta, const Board& b, PlayerColor color){

    Board newBoard = Board(b.data);
    cout<<"max, depth = "<<depth<<" color = "<<" alpha = "<<alpha<<" beta = "<<beta<<endl;
    int maxEval = -INFINITY;
    int eval;
    unordered_set <U16> legalMoves = b.get_legal_moves();
    for (const auto move : legalMoves){
        Board newBoard2 = Board(b.data);
        cout<<"move :"<<move_to_str(move)<<endl;
        newBoard2.do_move_(move);
        eval = alphaBetaPrune(depth-1, alpha, beta, newBoard2, false, color);
        maxEval = max(maxEval, eval);
        alpha = max(alpha, maxEval);
        cout<<"after one move max, depth = "<<depth<<" alpha = "<<alpha<<" beta = "<<beta<<" eval = "<<eval<<" maxEval = "<<maxEval<<endl;
        if (beta <= alpha){
            break;
        }
    
    }
    return maxEval;
}

U16 alphaBetaPrune(int depth, int alpha, int beta,const Board& b, bool maximisingPlayer, PlayerColor color){
    if (depth == 0){
        return evaluate(b, color);
    }
    unordered_set <U16> legalMoves = b.get_legal_moves();
    if (maximisingPlayer){
    Board newBoard = Board(b.data);
    cout<<"max, depth = "<<depth<<" color = "<<" alpha = "<<alpha<<" beta = "<<beta<<endl;
    int maxEval = -INFINITY;
    int eval;
    // unordered_set <U16> legalMoves = b.get_legal_moves();
    for (const auto move : legalMoves){
        Board newBoard2 = Board(b.data);
        cout<<"move :"<<move_to_str(move)<<endl;
        newBoard2.do_move_(move);
        eval = alphaBetaPrune(depth-1, alpha, beta, newBoard2, false, color);
        maxEval = max(maxEval, eval);
        alpha = max(alpha, maxEval);
        cout<<"after one move max, depth = "<<depth<<" alpha = "<<alpha<<" beta = "<<beta<<" eval = "<<eval<<" maxEval = "<<maxEval<<endl;
        if (beta <= alpha){
            break;
        }
    
    }
    return maxEval;
    }else{
    Board newBoard = Board(b.data);

    // if (depth == 0){
    //     return evaluate(newBoard, color);
    // }
    int minEval = INFINITY;
    int eval;
    // unordered_set <U16> legalMoves = b.get_legal_moves();
    for (const auto move : legalMoves){
        Board newBoard2 = Board(b.data);
        cout<<"move :"<<move_to_str(move)<<endl;
        newBoard2.do_move_(move);
        // if (color == WHITE){
        //     // eval = maxValue(depth-1, alpha, beta, newBoard2, BLACK);
        //     eval = alphaBetaPrune(depth-1, alpha, beta, newBoard2, true, BLACK);
        // }else{
        //     eval = alphaBetaPrune(depth-1, alpha, beta, newBoard2, true, WHITE);
        // }
        eval = alphaBetaPrune(depth-1, alpha, beta, newBoard2, true, color);
        
        
        minEval = min(minEval, eval);
        beta = min(beta, minEval);
        cout<<"after one move min, depth = "<<depth<<" alpha = "<<alpha<<" beta = "<<beta<<" eval = "<<eval<<" minEval = "<<minEval<<endl;
        if (beta <= alpha){
            break;
        }
    
    }
    return minEval;
    
    }

}

bool position_under_attack(Board& b, U8 pos, PlayerColor color) {
    unordered_map<PlayerColor, PlayerColor> oppositeColor;
    // unordered_set<U8> ourPositions;
    oppositeColor[WHITE] = BLACK;
    oppositeColor[BLACK] = WHITE;
    b.data.player_to_play = oppositeColor[color];
    auto oppLegalMoves = b.get_legal_moves();
    for (auto move : oppLegalMoves){
        if (getp1(move) == pos){
            return true;
        }
    }
    return false;

}

void Engine::find_best_move(const Board& b) {
    
    unordered_map<U16, int> points;
    points[WHITE | PAWN] = 1;
    points[WHITE | ROOK] = 100;
    points[WHITE | BISHOP] = 10;
    points[WHITE | KING] = 0;

    points[BLACK | PAWN] = 1;
    points[BLACK | ROOK] = 100;
    points[BLACK | BISHOP] = 10;
    points[BLACK | KING] = 0;
    PlayerColor color = b.data.player_to_play;
    // U8 ourAttackedPiecePos;
    auto move1 = move(pos(4, 1), pos(5, 1));

    // pick a random move
    
    auto moveset = b.get_legal_moves();

    // cout<<"modda gudu\n"<<endl;
    // if (b.data.board_0[getp1(move1)] == color | ROOK){
    //     this->best_move = move1;
    // }
    bool check;
    if (moveset.size() == 0) {
        std::cout << "Could not get any moves from board!\n";
        std::cout << board_to_str(&b.data);
        this->best_move = 0;
    }
    else {
        cout<<"modda gudu\n"<<endl;
        std::cout << board_to_str(&b.data);
        unordered_map<PlayerColor, PlayerColor> oppositeColor;
        oppositeColor[WHITE] = BLACK;
        oppositeColor[BLACK] = WHITE;
        int alpha = -10000;
        int beta = 10000;
        int bestEval = -10000;
        int eval = 0;
        int depth = 0;
        U16 best_move;
        U8 final_pos;
        U8 final_piece;
        U8 initial_pos;
        U8 initial_piece;
        // Board b2 = Board(b.data);
        // ourAttackedPiecePos = attackOnUs(b2, color);
        
        // if (ourAttackedPiecePos != 0){
        //     check = true;
        // }
        // if (check){
        //     moveset = b.get_pseudolegal_moves();
        // }
    vector<U16> shuffled_elements(moveset.begin(), moveset.end());
    random_device rd;
    mt19937 g(rd());
    shuffle(shuffled_elements.begin(), shuffled_elements.end(), g);
    moveset.clear();
    for (const auto& elem : shuffled_elements) {
        moveset.insert(elem);
    }

        for (auto move : moveset) {

            // our attack
            initial_pos = getp0(move);
            initial_piece = b.data.board_0[initial_pos];
            final_pos = getp1(move);
            if (b.data.board_0[final_pos] != EMPTY){
                cout<<"final pos is not empty\n"<<endl;
                final_piece = b.data.board_0[final_pos];
                if (final_piece != EMPTY && (final_piece & oppositeColor[color])){
                    if (points[final_piece] > points[initial_piece]){
                        cout<<"we are attacking\n"<<endl;
                        best_move = move;
                        break;
                    }
                }
            }
            // our defense
            Board defenseBoard = Board(b.data);
            if (position_under_attack(defenseBoard, final_pos, color)){
                continue;
            }

            Board newBoard = Board(b.data);
            // cout<<"move :"<<move_to_str(move)<<endl;
            newBoard.do_move_(move);
            if (newBoard.get_legal_moves().size() == 0){
                this->best_move = move;
                break;
            }
            // cout<<"initial color :"<<color<<endl;
            eval = alphaBetaPrune(depth, alpha, beta, newBoard, false, color);
            // cout<<"eval :"<<eval<<endl<< "best eval :"<<bestEval<<endl;
            if (eval >= bestEval){
                cout<<"updated best move :"<<move_to_str(move)<<endl;
                bestEval = eval;
                best_move = move;
                // this->best_move = best_move;
            }
        }
        if (best_move == 0){
            std::vector<U16> moves;
            cout<<"best move is 0"<<endl;
                    // std::cout << std::endl;
            std::sample(
            moveset.begin(),
            moveset.end(),
            std::back_inserter(moves),
            1,
            std::mt19937{std::random_device{}()}
        );
        this->best_move = moves[0];
        }
        this->best_move = best_move;
    }

    // just for debugging, to slow down the moves
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
}


// U8 attackOnUs(Board& b, PlayerColor color){
//     unordered_map<PlayerColor, PlayerColor> oppositeColor;
//     oppositeColor[WHITE] = BLACK;
//     oppositeColor[BLACK] = WHITE;
//     b.data.player_to_play = oppositeColor[color];
//     auto oppLegalMoves = b.get_legal_moves();
//     for (auto move : oppLegalMoves){
//         if (b.data.board_0[getp1(move)] == color | KING){
//             return getp1(move);
//         }
//         if (b.data.board_0[getp1(move)] == color | ROOK){
//             return getp1(move);
//         }
//         if(b.data.board_0[getp1(move)] == color | BISHOP){
//             return getp1(move);
//         }
//         if(b.data.board_0[getp1(move)] == color | KNIGHT){
//             return getp1(move);
//         }
//         if (b.data.board_0[getp1(move)] == color | PAWN){
//             return getp1(move);
//         }

//     }
//     return 0;
// }


int evaluate(const Board& b, PlayerColor color){
    cout<<"eval color = "<<color<<endl;
    std::cout << board_to_str(&b.data);
    int white_score = 0;
    int black_score = 0;
    // color = b.data.player_to_play;
    unordered_map<U16, int> points;
    points[WHITE | PAWN] = 1;
    points[WHITE | ROOK] = 100;
    points[WHITE | BISHOP] = 10;
    points[WHITE | KING] = 1000;

    points[BLACK | PAWN] = 1;
    points[BLACK | ROOK] = 100;
    points[BLACK | BISHOP] = 10;
    points[BLACK | KING] = 1000;

    if (color == WHITE){
        // white_score += 100*(gety(b.data.w_rook_1)-1);
        // white_score += 100*(gety(b.data.w_rook_2)-1);
        // white_score += 10*(getx(b.data.w_rook_1));
        // white_score += 10*(getx(b.data.w_rook_2));
        // white_score += 100*(gety(b.data.w_knight_1)-1);

        if(b.data.w_rook_1 != DEAD){
            if ((b.data.w_rook_1) == pos(1,0)){
                white_score += 1000;
            }
            if (b.data.w_rook_1 == pos(0,6)){
                white_score += 1000;
            }

            if (b.data.w_rook_1 == pos(6, 6)){
                white_score += 1000;
            }
        }

        if (b.data.w_rook_2 != DEAD){
            if (b.data.w_rook_2 == pos(1, 0)){
                white_score += 1000;
            }
            if (b.data.w_rook_2 == pos(0, 6)){
                white_score += 1000;
            }
            if (b.data.w_rook_2 == pos(6, 6)){
                white_score += 1000;
            }
        }

        U8 b_king_pos = getp1(b.data.b_king);
        U8 w_king_pos = getp1(b.data.w_king);
        Board b2 = Board(b.data);
        bool check1 = position_under_attack(b2, b_king_pos, color);
        bool check2 = position_under_attack(b2, w_king_pos, color);
        if (check1){
            white_score += 1000;
        }
        if (check2){
            white_score -= 1000;
        }

    }else{
        if (b.data.b_rook_1 != DEAD){
            if ((b.data.b_rook_1) == pos(1,0)){
                black_score += 1000;
            }
            if (b.data.b_rook_1 == pos(0,6)){
                black_score += 1000;
            }

            if (b.data.b_rook_1 == pos(6, 6)){
                black_score += 1000;
            }
        }

        if(b.data.b_rook_2 != DEAD){

            if (b.data.b_rook_2 == pos(1, 0)){
                black_score += 1000;
            }
            if (b.data.b_rook_2 == pos(0, 6)){
                black_score += 1000;
            }
            if (b.data.b_rook_2 == pos(6, 6)){
                black_score += 1000;
            }
        }

        
    //     Board b2 = Board(b.data);
    //     bool check1 = position_under_attack(b2, b_king_pos, color);
    //     bool check2 = position_under_attack(b2, w_king_pos, color);
    //     if (check1){
    //         black_score += 1000;
    //     }
    //     if (check2){
    //         black_score -= 1000;
    //     }
    // }



    // if (color == WHITE){
    //     white_score += 100*(gety(b.data.w_pawn_1)-1);
    //     white_score += 100*(gety(b.data.w_pawn_2));
    // }
    // else{
    //     black_score += 100*(gety(b.data.b_pawn_1)-1);
    //     black_score += 100*gety(b.data.b_pawn_2);
    // }


    for(int i = 0; i < 64; i++){
        U8 every_piece = b.data.board_0[i];
        if(every_piece != EMPTY && (every_piece & BLACK)){  //black piece
            black_score += points[every_piece];
        }
        if(every_piece != EMPTY && (every_piece & WHITE)){  //white piece
            white_score += points[every_piece];
        }
    }

    if (color == WHITE){
        cout<<"white score :"<<white_score<<"black score :"<<black_score<<"evaluuation score : "<<white_score-black_score<<"\n";
        return white_score-black_score;
    }else{
        cout<<"whit score :"<<white_score<<"blac score :"<<black_score<<"evaluuation score : "<<black_score-white_score<<"\n";
        return black_score-white_score;
    }
}
}





