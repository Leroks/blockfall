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
    game.gameEnd = 0;

    fstream inputFile(commands_file);
    if (!inputFile.is_open()) {
        cout << "Failed to open the file." << endl;
        return false;
    }
    string line;
    if (game.active_rotation == nullptr) {
        game.gameEnd = 1;
    }
    if (checkCollision(game.pos, game)) {
        game.gameEnd = 2;

    }

    while (getline(inputFile, line)) {
        if (game.gameEnd != 0) break;
        if (line == "ROTATE_LEFT") {
            if (!checkCollision(game.pos, game)) {
                game.active_rotation = game.active_rotation->left_rotation;
            }
        } else if (line == "ROTATE_RIGHT") {
            if (!checkCollision(game.pos, game)) {
                game.active_rotation = game.active_rotation->right_rotation;
            }
        } else if (line == "MOVE_LEFT") {
            if (!checkCollision(game.pos, game)) {
                game.pos--;
            }
        } else if (line == "MOVE_RIGHT") {
            if (!checkCollision(game.pos, game)) {
                game.pos++;
            }
        } else if (line == "DROP") {
            if (game.gravity_mode_on) {
                dropWithGravity(game);
            } else if (!game.gravity_mode_on) {
                dropWithoutGravity(game);
            }

            if (checkPowerUp(game)) {
                usePowerUp(game);
            }

            bool isClear = true;
            for (int i = 0; i < game.grid.size(); i++) {
                if (isRowFull(game, i)) {
                    if (isClear) {

                        cout << "Before clearing:" << endl;
                        printEmptyGrid(game, false);
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
            if (checkCollision(game.pos, game)) {
                game.gameEnd = 2;
            }
        } else if (line == "") {
            continue;
        } else if (line == "GRAVITY_SWITCH") {
            changeGravity(game);
        } else if (line == "PRINT_GRID") {
            printGrid(game);
        } else {
            cout << "Unknown command: " << line << endl;
        }

        if (game.active_rotation == nullptr) {
            game.gameEnd = 1;
        }
        if (checkCollision(game.pos, game)) {
            game.gameEnd = 2;
        }
    }

    bool ans = false;


    if (game.gameEnd == 1) {
        cout << "YOU WIN!\nNo more blocks.\nFinal grid and score:\n" << endl;
        ans = true;
    } else if (game.gameEnd == 0) {
        cout << "GAME FINISHED!\nNo more commands.\nFinal grid and score:\n" << endl;
        ans = true;
    } else if (game.gameEnd == 2) {
        cout << "GAME OVER!\nNext block that couldn't fit:" << endl;
        vector<vector<bool>> x = game.active_rotation->shape;
        for (int i = 0; i < x.size(); i++) {
            for (int j = 0; j < x[i].size(); j++) {
                if (x[i][j] == 1) {
                    cout << occupiedCellChar;
                } else {
                    cout << unoccupiedCellChar;
                }
            }
            cout << endl;
        }
        cout << "\nFinal grid and score:\n" << endl;
    }
    game.leaderboard.insert_new_entry(new LeaderboardEntry(game.current_score, std::time(nullptr), game.player_name));
    cout << "Score: " << game.current_score << endl;
    if (game.leaderboard.head_leaderboard_entry != nullptr) {
        int high = max(game.current_score, game.leaderboard.head_leaderboard_entry->score);
        cout << "High Score: " << high << endl;
    } else {
        cout << "High Score: " << game.current_score << endl;
    }
    printEmptyGrid2(game);
    cout << endl;
    game.leaderboard.print_leaderboard();
    cout << endl;
    cout << endl;
    cout << endl;
    game.leaderboard.write_to_file(game.leaderboard_file_name);
    return ans;

}

void GameController::printGrid(BlockFall &game) {
    cout << "Score: " << game.current_score << endl;
    if (game.leaderboard.head_leaderboard_entry != nullptr) {
        int high = max(game.current_score, game.leaderboard.head_leaderboard_entry->score);
        cout << "High Score: " << high << endl;
    } else {
        cout << "High Score: " << game.current_score << endl;
    }
    if (game.active_rotation == nullptr || checkCollision(game.pos, game) || game.gameEnd != 0) {
        printEmptyGrid(game, false);
    } else {
        printEmptyGrid(game, true);
    }

}

void GameController::changeGravity(BlockFall &game) {
    game.gravity_mode_on = !game.gravity_mode_on;
    if (game.gravity_mode_on) {
        //make everything fall
        for (int i = game.grid.size() - 1; i >= 0; i--) {
            for (int j = 0; j < game.grid[i].size(); j++) {
                if (game.grid[i][j] == 1) {
                    int k = i;
                    while (k < game.grid.size() - 1 && game.grid[k + 1][j] == 0) {
                        game.grid[k + 1][j] = 1;
                        game.grid[k][j] = 0;
                        k++;
                    }
                }
            }
        }
        for (int i = 0; i < game.grid.size(); i++) {
            if (isRowFull(game, i)) {

                game.current_score += clearRow(game, i);
            }
        }
    }
}

bool GameController::checkCollision(long pos, BlockFall &fall) {
    vector<vector<bool>> shape = fall.active_rotation->shape;
    if (fall.active_rotation == nullptr) {
        return false;
    }
    for (int i = 0; i < fall.grid.size(); i++) {
        for (int j = 0; j < fall.grid[i].size(); j++) {
            if (shape[i][j] == 1 && fall.grid[i][j] == 1) {
                return true;
            }
        }
    }

    if (pos + shape[0].size() > fall.grid[0].size()) {
        return true;
    }

    return false;
}

unsigned long GameController::clearRow(BlockFall &fall, int row) {
    int sum = 0;
    for (int i = row; i > 0; i--) {
        for (int j = 0; j < fall.grid[i].size(); j++) {
            fall.grid[i][j] = fall.grid[i - 1][j];
        }
    }
    sum = fall.grid[0].size();
    for (int i = 0; i < fall.grid[0].size(); i++) {
        fall.grid[0][i] = 0;
    }
    return sum;
}

void GameController::dropWithGravity(BlockFall &fall) {
    vector<vector<bool>> x = fall.active_rotation->shape;
    int highest = 999999;
    int lowest = 9999;
    int counter = 0;
    for (int i = x.size() - 1; i >= 0; i--) {
        for (int j = 0; j < x[i].size(); j++) {
            if (x[i][j] == 1) {
                highest = downAmount(fall, i, j);
                fall.grid[i + highest][j + fall.pos] = 1;
                counter++;
                lowest = min(lowest, highest);
            }
        }
    }
    fall.current_score += lowest * counter;
}

int GameController::downAmountShape(BlockFall &fall) {
    vector<vector<bool>> x = fall.active_rotation->shape;
    int ans = 1000000;
    for (int i = 0; i < x.size(); i++) {
        for (int j = 0; j < x[i].size(); j++) {
            if (x[i][j] == 1) {
                ans = min(ans, downAmount(fall, i, j));
            }
        }
    }
    return ans;
}

void GameController::dropWithoutGravity(BlockFall &fall) {
    vector<vector<bool>> x = fall.active_rotation->shape;
    int ans = downAmountShape(fall);
    int score = 0;
    for (int i = x.size() - 1; i >= 0; i--) {
        for (int j = 0; j < x[i].size(); j++) {
            if (x[i][j] == 1) {
                fall.grid[i + ans][j + fall.pos] = 1;
                score += ans;
            }
        }
    }
    fall.current_score += score;
}

bool GameController::checkPowerUp(BlockFall &fall) {
    vector<vector<bool>> x = fall.power_up;
    vector<vector<int>> y = fall.grid;
    for (int i = 0; i <= y.size() - x.size(); i++) {
        for (int j = 0; j <= y[0].size() - x[0].size(); j++) {
            bool trial = true;
            for (int k = 0; k < x.size(); k++) {
                for (int l = 0; l < x[0].size(); l++) {
                    if (x[k][l] != y[i + k][j + l]) {
                        trial = false;
                    }
                }
            }
            if (trial) {
                return true;
            }
        }
    }
    return false;
}

void GameController::usePowerUp(BlockFall &fall) {
    //first2 = false;
    cout << "Before clearing:" << endl;
    printEmptyGrid(fall, false);
    int sum = clear(fall);
    fall.current_score += sum + 1000;
}

void GameController::printEmptyGrid(BlockFall &fall, bool b) {
    if (!b) {
        printEmptyGrid2(fall);
    } else {
        vector<vector<bool>> x = fall.active_rotation->shape;

        for (int i = 0; i < fall.rows; i++) {
            for (int j = 0; j < fall.cols; j++) {
                int k = j - fall.pos;
                int z = 0;
                if (k >= 0 && k < x[0].size() && i < x.size()) {
                    z = x[i][k];
                }
                if (fall.grid[i][j] == 1 || z == 1) {
                    cout << occupiedCellChar;
                } else {
                    cout << unoccupiedCellChar;
                }
            }
            cout << endl;
        }
    }
    cout << endl;
    cout << endl;
}

void GameController::printEmptyGrid2(BlockFall &fall) {
    for (int i = 0; i < fall.rows; i++) {
        for (int j = 0; j < fall.cols; j++) {
            if (fall.grid[i][j] == 1) {
                cout << occupiedCellChar;
            } else {
                cout << unoccupiedCellChar;
            }
        }
        cout << endl;
    }

}

int GameController::clear(BlockFall &fall) {
    int sum = 0;
    for (int i = 0; i < fall.grid.size(); i++) {
        for (int j = 0; j < fall.grid[i].size(); j++) {
            sum += fall.grid[i][j];
            fall.grid[i][j] = 0;
        }
    }
    return sum;
}

bool GameController::isRowFull(BlockFall &fall, int row) {
    for (int i = 0; i < fall.grid[row].size(); i++) {
        if (fall.grid[row][i] == 0) {
            return false;
        }
    }
    return true;
}






