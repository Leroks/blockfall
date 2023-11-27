#include "GameController.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


int GameController::downAmount(BlockFall &game, int x, int y) {
    int ans = -1;
    long int mainY = y + game.pos;
    for (int i = x; i < game.grid.size(); i++) {
        if (game.grid[i][mainY] == 1) {
            break;
        }
        ans++;
    }
    return ans;
}

bool GameController::play(BlockFall &game, const string &commands_file) {

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    fstream inputFile(commands_file);
    if (!inputFile.is_open()) {
        cout << "Failed to open the file." << endl;
        return false;
    }
    string line;

    while (getline(inputFile, line)) {
        if (game.gameEnd != 0) break;
        if (line == "ROTATE_LEFT") {
            game.active_rotation = game.active_rotation->left_rotation;
        } else if (line == "ROTATE_RIGHT") {
            game.active_rotation = game.active_rotation->right_rotation;
        } else if (line == "MOVE_LEFT") {
            game.pos--;
        } else if (line == "MOVE_RIGHT") {
            game.pos++;
        } else if (line == "DROP") {
            if (game.gravity_mode_on) {
                int counter = 0;
                int highest = 999999;
                int lowest = 9999;
                vector<vector<bool>> shape = game.active_rotation->shape;
                for (int i = shape.size() - 1; i >= 0; i--) {
                    for (int j = 0; j < shape[i].size(); j++) {
                        if (shape[i][j] == 1) {
                            int total = 1;
                            int t = j + game.pos;
                            for (int k = i; k < game.grid.size(); k++) {
                                if (game.grid[k][t] == 1) {
                                    break;
                                }
                                total++;
                            }
                            highest = total;
                            game.grid[i + highest][j + game.pos] = 1;
                            counter++;
                            lowest = min(lowest, highest);
                        }
                    }
                }
                game.current_score += counter * lowest;
            } else {
                int total = 0;
                vector<vector<bool>> shape = game.active_rotation->shape;
                int highest = 999999;
                for (int i = 0; i < shape.size(); i++) {
                    for (int j = 0; j < shape[i].size(); j++) {
                        if (shape[i][j] == 1) {
                            highest = min(highest, downAmount(game, i, j));
                        }
                    }
                }
                for (int i = shape.size() - 1; i >= 0; i--) {
                    for (int j = 0; j < shape[i].size(); j++) {
                        if (shape[i][j] == 1) {
                            game.grid[i + highest][j + game.pos] = 1;
                            total += highest;
                        }
                    }
                }
            }

            /*if (powerDetection(game)) {
                power_up(game);
            }*/


            bool isClear = true;
            for (int i = 0; i < game.grid.size(); i++) {
                if (checkFullRow(game, i)) {
                    if (isClear){

                        cout << "Before clearing:" << endl;
                        for (int l = 0; l < game.rows; l++) {
                            for (int j = 0; j < game.cols; j++) {
                                if (game.grid[l][j] == 1 ) {
                                    cout << occupiedCellChar;
                                } else {
                                    cout << unoccupiedCellChar;
                                }
                            }
                            cout << endl;
                        }
                        isClear = false;
                    }

                    game.current_score += clearRow(game, i);
                }
            }
            game.active_rotation = game.active_rotation->next_block;
            game.pos = 0;

            if (game.active_rotation == nullptr) {
                game.gameEnd = 1;
            }
            if ( checkCollision( game.pos,game)) {
                game.gameEnd = 2;
            }

        } else if (line == "GRAVITY_SWITCH") {

        } else if (line == "PRINT_GRID") {

        } else {
            cout << "Invalid command" << endl;
        }
    }

    if (game.gameEnd == 2) {
        cout << "GAME OVER!\nNext block that couldn't fit:" << endl;
        vector<vector<bool>> shape = game.active_rotation->shape;
        for (auto &i: shape) {
            for (auto &&j: i) {
                if (j) {
                    cout << "██";
                } else {
                    cout << "▒▒";
                }
            }
            cout << endl;
        }
        cout << "\nFinal grid and score:\n" << endl;
    }
    if (game.gameEnd == 1) {
        cout << "YOU WIN!\nNo more blocks.\nFinal grid and score:\n" << endl;
        return true;
    }
    if (game.gameEnd == 0) {
        cout << "GAME FINISHED!\nNo more commands.\nFinal grid and score:\n" << endl;
        return true;
    }


    return false;
}

bool GameController::checkCollision(long pos, BlockFall &fall) {
    vector<vector<bool>> shape = fall.active_rotation->shape;
    if(fall.active_rotation == nullptr){
        return false;
    }
    for(int i = 0; i< fall.grid.size(); i++){
        for(int j = 0; j< fall.grid[i].size(); j++){
            if(shape[i][j] == 1 && fall.grid[i][j] == 1){
                return true;
            }
        }
    }

    if(pos + shape[0].size() > fall.grid[0].size()){
        return true;
    }

    return false;
}

bool GameController::checkFullRow(BlockFall &fall, int row) {
    for (int i = 0; i < fall.grid[row].size(); i++) {
        if (fall.grid[row][i] == 0) {
            return false;
        }
    }
    return true;
}

unsigned long GameController::clearRow(BlockFall &fall, int row) {
    int sum = 0;
    for (int i = row; i > 0; i--) {
        for (int j = 0; j < fall.grid[i].size(); j++) {
            fall.grid[i][j] = fall.grid[i-1][j];
        }
    }
    sum = fall.grid[0].size();
    for (int i = 0; i < fall.grid[0].size(); i++) {
        fall.grid[0][i] = 0;
    }
    return sum;
}



