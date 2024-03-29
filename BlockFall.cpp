#include "BlockFall.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on,
                     const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {

    Block *currentBlock = new Block;

    fstream inputFile(input_file);
    if (!inputFile.is_open()) {
        cout << "Failed to open the file." << endl;
        return;
    }
    vector<bool> liner;
    char i;
    Block *previousBlock;
    long int determiner = 0;
    while (inputFile >> i) {
        if (i == '[') {
            if (initial_block == nullptr) {
                initial_block = currentBlock;
            } else {
                previousBlock = currentBlock;
                currentBlock = currentBlock->next_block;
            }
            currentBlock->shape.clear();

            string line;

            bool read = false;
            while (!read) {
                getline(inputFile, line);
                istringstream basicIstringstream(line);
                string search = "]";
                size_t found = line.find(search);
                if (found != string::npos) {
                    read = true;
                }
                char number;
                while (basicIstringstream >> number) {
                    if (number == '0') {
                        liner.push_back(false);
                    }
                    if (number == '1') {
                        liner.push_back(true);
                    }
                    if (number == ']') {
                        break;
                    }
                }
                currentBlock->shape.push_back(liner);
                liner.clear();
            }
            currentBlock->next_block = new Block;
            currentBlock->createCircularLinkedList();
        }
    }

    if (!inputFile.is_open()) {
        delete currentBlock;
        return;
    }
    delete currentBlock->next_block;
    if (initial_block == currentBlock) {
        initial_block = nullptr;
    }
    int rotationIndex1 = 0;
    int rotationIndex2 = 1;
    long int rotationIndex = rotationIndex1 * rotationIndex2;
    while (rotationIndex < 4) {
        currentBlock->next_block = nullptr;
        currentBlock = currentBlock->left_rotation;
        if (previousBlock != nullptr) {
            previousBlock->next_block = nullptr;
            previousBlock = previousBlock->left_rotation;
        }
        rotationIndex *= rotationIndex2;
        rotationIndex += rotationIndex1;
        rotationIndex++;
    }
    if (initial_block != nullptr) {
        active_rotation = initial_block;
    }
    power_up = currentBlock->shape;
    currentBlock->left_rotation->right_rotation = nullptr;
    currentBlock->left_rotation = nullptr;
    while (currentBlock != nullptr) {
        if(currentBlock == initial_block){
            break;
        }
        Block *pBlock = currentBlock;
        currentBlock = currentBlock->right_rotation;
        delete pBlock;
    }


    inputFile.close();
}


void BlockFall::initialize_grid(const string &input_file) {
    fstream inputFile(input_file);
    if (inputFile.is_open()) {
        string line;
        getline(inputFile, line);
        istringstream iss(line);
        string num;
        int count = 0;
        while (iss >> num) {
            count++;
        }
        cols = count;
        while (getline(inputFile, line)) {
            rows++;
        }
        inputFile.close();
    }
    fstream inputFile2(input_file);
    if (inputFile2.is_open()) {
        string line;
        while (getline(inputFile2, line)) {
            istringstream iss(line);
            vector<int> lineData;
            int num;
            while (iss >> num) {
                lineData.push_back(num);
            }
            grid.push_back(lineData);
        }
        inputFile2.close();
    }
}


BlockFall::~BlockFall() {
    Block *pBlock = initial_block;
    while (pBlock != nullptr) {
        if(pBlock == initial_block){
            break;
        }
        Block *pBlock2 = pBlock;
        pBlock2->left_rotation->right_rotation = nullptr;
        pBlock2->left_rotation = nullptr;
        while (pBlock2 != nullptr) {
            if(pBlock2 == initial_block){
                break;
            }
            pBlock2 = pBlock2->right_rotation;
        }
        pBlock = pBlock->next_block;
    }
}
