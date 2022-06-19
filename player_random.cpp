#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <bits/stdc++.h>
using namespace std;
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player, opp;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
string player_type[3];

int calculate_score(string input, int left, int right, int valid_range){
    int current_score = 0;
    string compare; compare.push_back(input[4]); 
    int defense = (compare == "o"), len = left - right + 1;
    char op = (input[4] == 'o')? 'x' : 'o';
    bool left_three = false, right_three = false;
    if (len >= 5){
        if (defense) return 10000000; 
        else return -1000000;
    }
    // else {
    //     bool left_is_empty = (input[left+1] == '.'), right_is_empty = (input[right-1] == '.');
    //     if (!left_is_empty&&!right_is_empty){
    //         if (defense) return -1000; 
    //         else return 1000;
    //     }
    // }
    else if (len == 4){
        bool left_is_empty = (input[left+1] == '.'), right_is_empty = (input[right-1] == '.');
        if (left_is_empty && right_is_empty){
            if (defense) current_score = -100000;
            else current_score = 50000;
        }
        else {
            if (defense) current_score = -100000;
            else current_score = 400;
        }
    }
    else if (len == 3){
        bool left_is_empty = (input[left+1] == '.'), right_is_empty = (input[right-1] == '.'); 
        if (left_is_empty){
            if (input[left+2] == input[4]){
                if (defense) current_score = -100000; //ooo.o
                else current_score = 400;
            }
        }
        if (right_is_empty){
            if (input[left-2] == input[4]){         //o.ooo
                if (defense) current_score = -100000;
                else current_score = 400;
            }
        }
        if (left_is_empty && right_is_empty){
            if (valid_range > 5){               //.ooo..
                if (defense) current_score = -8000;
                else current_score = 400;
            }
            else {                            //.ooo.x
                if (defense) current_score = -50;
                else current_score = 20;
            }
        }
        else {                              
            if (defense) current_score = -50;
            else current_score = 20;
        }
    }
    else if (len == 2){
	    left_three = right_three = false;
        bool left_is_empty = (input[left+1] == '.'), right_is_empty = (input[right-1] == '.'); 
        if (right_is_empty){
            if (input[right-2] == input[4]){
                if (input[right-3] == '.'){
                    if (input[left+1] == '.'){ // XMXMMX
                        if (defense) current_score = -8000;
                        else current_score = 400;
                    }
                    else{           // XMXMMP
                        if (defense) current_score = -50;
                        else current_score = 20;
                    }
                    right_three = true;
                }
                else if (input[right-3] == op){ // PMXMMX
                    if (input[left+1] == '.'){
                        if (defense) current_score = -50;
                        else current_score = 20;
                        left_three = true;
                    }
                }
            }
        }
        if (left_is_empty){
            if (input[left+2] == input[4]){
                if (input[left+3] == input[4]){  // MMXMM
                    if (defense) current_score = -100000;
                    else current_score = 400;
                    left_three = true;
                }
                else if (input[left+3] == '.'){
                    if (right_is_empty){  //XMMXMX
                        if (defense) current_score = -8000;
                        else current_score = 400;
                    }
                    else{ // PMMXMX
                        if (defense) current_score = -50;
                        else current_score = 20;
                    }
                    left_three = true;
                }
                else if (left_is_empty){ // XMMXMP
                    if (defense) current_score = -50;
                    else current_score = 20;
                    left_three = true;
                }
            }
        }
        if (left_three || right_three){}
        else if (left_is_empty && right_is_empty){ // XMMX
            if (defense) current_score = -50;
            else current_score = 20;
        }
        else if (left_is_empty || right_is_empty){ // PMMX, XMMP
            if (defense) current_score = -3;
            else current_score = 2;
        }
    }
    else if (len == 1) {
        bool left_is_empty = (input[left+1] == '.'), right_is_empty = (input[right-1] == '.');
        if (right_is_empty){
            if (input[right-2] == input[4]){
                if (input[right-3] == '.'){
                    if (input[left+1] == op){  // XMXMP
                        if (defense) current_score = -3;
                        else current_score = 2;
                    }
                }
            }
        }
        if (left_is_empty){
            if (input[left+2] == input[4]){
                if (input[left+3] == '.'){
                    if (right_is_empty){// XMXMX
                        if (defense) current_score = -50;
                        else current_score = 20;
                    }
                    else{// PMXMX
                        if (defense) current_score = -3;
                        else current_score = 2; 
                    }
                }
            }
            else if(input[left+2] == '.'){
                if (input[left+3] == input[4] && input[left+4] == '.'){  //XMXXMX
                    if (defense) current_score = -50;
                    else current_score = 20;
                }
            }
        }
    }
    //cout << current_score << '\n';
    return current_score;
}

int heuristic(int who){
    string chess_road;
    int op = (who == 1)?2:1;
    int score = 0;
    int dir[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    player_type[0] = ".", player_type[who] = "o", player_type[op] = "x";
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if (board[i][j] == EMPTY) continue;
            for (int k = 0; k < 4; k++){
                for (int l = -4; l <= 4; l++){
                    if (i+dir[k][0]*l >= 0 && i+dir[k][0]*l < SIZE && j+dir[k][1]*l >= 0 && j+dir[k][1]*l < SIZE)
                        chess_road += player_type[board[i+dir[k][0]*l][j+dir[k][1]*l]];
                    else
                        chess_road += player_type[op];
                }
                //cout << chess_road << '\n';
                int left = 4, right = 4, l = 4, r = 4;
                while (chess_road[left] == chess_road[4] && left <= 8) left++;
                while (chess_road[right] == chess_road[4] && right >= 0) right--;
                while ((chess_road[l] == chess_road[4] || chess_road[l] == '.') && l <= 8) l++;
                while ((chess_road[r] == chess_road[4] || chess_road[r] == '.') && r >= 0) r--;
                score += calculate_score(chess_road, left-1, right+1, l-r-1);
                chess_road.clear();
            }
        }
    }
    //cout << score << '\n';
    return score;
}
struct state{
    int score, new_x, new_y;
    int new_chess;
    int chess_left;
    state(){
        chess_left = new_x = new_y = score = new_chess = 0;
    }
    void set_on_board(int col, int row, int who){
        new_x = col, new_y = row, new_chess = who;
        //score = heuristic(who);
    }
};

vector<state> generate_all_move(int who){
    int dir[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1 ,-1}};
    vector<state> all_possible_move;
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if (board[i][j] != EMPTY){
                for (int k = 0; k < 8; k++){
                    if (dir[k][0]+i >= 0 && dir[k][0]+i < SIZE && dir[k][1]+j >= 0 && dir[k][1]+j < SIZE && board[dir[k][0]+i][dir[k][1]+j] == EMPTY){
                        state tmp;
                        tmp.set_on_board(dir[k][0]+i, dir[k][1]+j, who);
                        all_possible_move.emplace_back(tmp);
                    }
                }
            }
        }
    }
    return all_possible_move;
}

//(score, (x, y))
state alpha_beta(state current, int depth, int alpha, int beta, int who){
    if (!depth || !current.chess_left){
        current.score = heuristic(who);
        //cout << current.new_x << ' ' << current.new_y << ' ' <<current.score << '\n';
        return current;
    }
    vector<state> all_moves = generate_all_move(who);
    if (who == player){ //1 is max
        state  max_evaluate;
        max_evaluate.score = INT_MIN;
        for (auto node:all_moves){
            state evaluate;
            board[node.new_x][node.new_y] = node.new_chess;
            node.chess_left--;
            evaluate = alpha_beta(node, depth-1, alpha, beta, opp);
            board[node.new_x][node.new_y] = EMPTY;
            node.chess_left++;
            if (max_evaluate.score < evaluate.score) max_evaluate = evaluate;
            alpha = max(alpha, evaluate.score);
            if (alpha >= beta) break;
        }
        return max_evaluate;
    }
    else {
        state min_evaluate;
        min_evaluate.score = INT_MAX;
        for (auto node:all_moves){
            state evaluate;
            board[node.new_x][node.new_y] = node.new_chess;
            node.chess_left--;
            evaluate = alpha_beta(node, depth-1, alpha, beta, player);
            board[node.new_x][node.new_y] = EMPTY;
            node.chess_left++;
            if (min_evaluate.score > evaluate.score) min_evaluate = evaluate;
            beta = min(beta, evaluate.score);
            if (alpha >= beta) break;
        }
        return min_evaluate;
    }
}

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void write_valid_spot(std::ofstream& fout) {
    int x, y;
    bool flag = false; //check whether the board is empty
    state initial;
    opp = (player == 1)?2:1;
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if (board[i][j] != EMPTY){
                flag = true;
                initial.chess_left++;
            }
        }
    }
    if (!flag) x = y = 7; //if is empty，choose the middle
    else {
        state now = alpha_beta(initial, 1, INT_MIN/2, INT_MAX/2, player);
        x = now.new_x;
        y = now.new_y;
    }
    if (board[x][y] == EMPTY) {
        fout << x << " " << y << std::endl;
        // Remember to flush the output to ensure the last action is written to file.
        fout.flush();
    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
