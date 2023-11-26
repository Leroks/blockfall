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
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
    fstream inputFile(input_file);
    Block* curr = new Block;
    Block * before ;
    std::vector<bool> lineData;
    bool isFile = false;

    if (inputFile.is_open()) {

        char tmp;
        while (inputFile >> tmp){
            if (tmp == '['){
                isFile = true;
                if (initial_block == nullptr){
                    initial_block = curr;
                }else{
                    before = curr;
                    curr = curr->next_block;

                }
                curr->shape.clear();
                std::string line;
                bool continueReading = true;
                int kl = 0;
                while (continueReading) {
                    kl++;
                    std::getline(inputFile, line);
                    std::istringstream iss(line);
                    std::string search = "]";
                    size_t found = line.find(search);
                    if (found != std::string::npos) {
                        continueReading = false;

                    }
                    char num;
                    while (iss >> num) {
                        if (num == '1'){
                            lineData.push_back(true);
                        }else if (num == '0'){
                            lineData.push_back(false);
                        }else if (num == ']'){
                            break;
                        }
                    }
                    curr->shape.push_back(lineData);

                    lineData.clear();

                }
                curr->next_block = new Block;
                curr->createCircularLinkedList();
            }


        }
    }else{
        std::cout << "Failed to open the file." << std::endl;

    }
    if (!isFile){
        delete curr;
        return;
    }
    delete curr->next_block;
    if (initial_block==curr){
        initial_block = nullptr;
    }
    for (int i = 0 ; i < 4 ; i++){
        curr->next_block = nullptr;
        curr = curr->left_rotation;
        if (before != nullptr){
            before->next_block = nullptr;
            before = before->left_rotation;
        }

    }
    power_up = curr->shape;
    //delete all rotations of the last block
    if (initial_block!= nullptr){
        active_rotation = initial_block;

    }
    curr->left_rotation->right_rotation = nullptr;
    curr->left_rotation = nullptr;
    while (curr != nullptr){
        Block *tmp = curr;
        curr = curr->right_rotation;
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
