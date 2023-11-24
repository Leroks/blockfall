#include "BlockFall.h"
#include <iostream>
#include <fstream>
#include <sstream>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name)
        : gravity_mode_on(gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    ifstream inputFile(input_file);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file: " << input_file << endl;
        return;
    }

    Block *currentBlock = nullptr;
    Block *previousBlock = nullptr;

    string line;
    while (getline(inputFile, line)) {
        if (line == "[") {
            if (currentBlock != nullptr) {
                previousBlock->next_block = nullptr;
                previousBlock->fillTheCircular();
            }

            currentBlock = new Block;
            if (initial_block == nullptr) {
                initial_block = currentBlock;
            } else {
                previousBlock = currentBlock;
                currentBlock = currentBlock->next_block;
            }

            currentBlock->shape.clear();

            while (getline(inputFile, line) && line != "]") {
                vector<bool> row;
                for (char c : line) {
                    if (c == '1') {
                        row.push_back(true);
                    } else if (c == '0') {
                        row.push_back(false);
                    }
                }
                currentBlock->shape.push_back(row);
            }
        }
    }

    if (currentBlock != nullptr) {
        delete currentBlock->next_block;
        currentBlock->left_rotation->right_rotation = nullptr;
        currentBlock->left_rotation = nullptr;
        power_up = currentBlock->shape;

        if (initial_block != nullptr) {
            active_rotation = initial_block;
        }

        delete currentBlock;
    }

    inputFile.close();
}

void BlockFall::initialize_grid(const string &input_file) {
    ifstream inputFile(input_file);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file: " << input_file << endl;
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        vector<int> row;

        int num;
        while (iss >> num) {
            row.push_back(num);
        }

        grid.push_back(row);
    }

    inputFile.close();
    rows = grid.size();
    cols = (rows > 0) ? grid[0].size() : 0;
}

BlockFall::~BlockFall() {
    Block *currentBlock = initial_block;
    while (currentBlock != nullptr) {
        Block *nextBlock = currentBlock->next_block;

        Block *rotation = currentBlock->left_rotation;
        while (rotation != nullptr) {
            Block *nextRotation = rotation->right_rotation;
            delete rotation;
            rotation = nextRotation;
        }

        delete currentBlock;
        currentBlock = nextBlock;
    }
}
