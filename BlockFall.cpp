#include "BlockFall.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    fstream inputFile(input_file);
    Block *currentBlock = new Block;
    Block *previousBlock;
    std::vector<bool> lineData;
    bool isFileOpen = false;

    if (!inputFile.is_open()) {
        std::cout << "Failed to open the file." << std::endl;
        return;
    }

    char tmp;
    while (inputFile >> tmp) {
        if (tmp == '[') {
            isFileOpen = true;
            if (initial_block == nullptr) {
                initial_block = currentBlock;
            } else {
                previousBlock = currentBlock;
                currentBlock = currentBlock->next_block;
            }
            currentBlock->shape.clear();
            std::string line;
            bool continueReading = true;
            int lineIndex = 0;
            while (continueReading) {
                lineIndex++;
                std::getline(inputFile, line);
                std::istringstream iss(line);
                std::string search = "]";
                size_t found = line.find(search);
                if (found != std::string::npos) {
                    continueReading = false;
                }
                char num;
                while (iss >> num) {
                    if (num == '1') {
                        lineData.push_back(true);
                    } else if (num == '0') {
                        lineData.push_back(false);
                    } else if (num == ']') {
                        break;
                    }
                }
                currentBlock->shape.push_back(lineData);
                lineData.clear();
            }
            currentBlock->next_block = new Block;
            currentBlock->createCircularLinkedList();
        }
    }

    if (!isFileOpen) {
        delete currentBlock;
        return;
    }
    delete currentBlock->next_block;
    if (initial_block == currentBlock) {
        initial_block = nullptr;
    }
    int rotationIndex = 0;
    while (rotationIndex < 4) {
        currentBlock->next_block = nullptr;
        currentBlock = currentBlock->left_rotation;
        if (previousBlock != nullptr) {
            previousBlock->next_block = nullptr;
            previousBlock = previousBlock->left_rotation;
        }
        rotationIndex++;
    }
    power_up = currentBlock->shape;
    if (initial_block != nullptr) {
        active_rotation = initial_block;
    }
    currentBlock->left_rotation->right_rotation = nullptr;
    currentBlock->left_rotation = nullptr;
    while (currentBlock != nullptr) {
        Block *tmp = currentBlock;
        currentBlock = currentBlock->right_rotation;
        delete tmp;
    }
}


void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main

    fstream inputFile(input_file);
    if(inputFile.is_open()){
        string line;
        getline(inputFile, line);
        istringstream iss(line);
        string num;
        int count = 0;
        while(iss >> num){
            count++;
        }
        cols = count;
        while(getline(inputFile, line)){
            rows++;
        }
        inputFile.close();
    }
    fstream inputFile2(input_file);
    if(inputFile2.is_open()){
        string line;
        while(getline(inputFile2, line)){
            istringstream iss(line);
            vector<int> lineData;
            int num;
            while(iss >> num){
                lineData.push_back(num);
            }
            grid.push_back(lineData);
        }
        inputFile2.close();
    }
}


BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    Block *tmp = initial_block;
    while (tmp != nullptr){
        Block *tmp2 = tmp;
        tmp2->left_rotation->right_rotation = nullptr;
        tmp2->left_rotation = nullptr;
        tmp = tmp->next_block;
        while (tmp2 != nullptr){
            Block *tmp3 = tmp2;
            tmp2 = tmp2->right_rotation;
            //delete shape of tmp3 here

            delete tmp3;
        }
    }
}
