#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block *right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block *left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block *next_block = nullptr; // Pointer to the next block to appear in the game

    bool operator==(const Block &other) const {
        // TODO: Overload the == operator to compare two blocks based on their shapes
        if (shape[0].size() != other.shape[0].size() || shape.size() != other.shape.size()) {
            return false;
        }
        for (int i = 0; i < shape.size(); i++) {
            for (int j = 0; j < shape[0].size(); j++) {
                if (shape[i][j] != other.shape[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(const Block &other) const {
        // TODO: Overload the != operator to compare two blocks based on their shapes
        if (shape[0].size() != other.shape[0].size() || shape.size() != other.shape.size()) {
            return true;
        }
        for (int i = 0; i < shape.size(); i++) {
            for (int j = 0; j < shape[0].size(); j++) {
                if (shape[i][j] != other.shape[i][j]) {
                    return true;
                }
            }
        }
        return false;
    }

    void createCircularLinkedList() {
        Block *start = this;
        Block *current = start;
        vector<vector<bool>> tempShape;

        for (int i = start->shape[0].size() - 1; i >= 0; --i) {
            vector<bool> newRow;
            newRow.reserve(start->shape.size());
            for (int j = 0; j < start->shape.size(); ++j) {
                newRow.push_back(start->shape[start->shape.size() - 1 - j][i]);
            }
            tempShape.push_back(newRow);
        }

        int counter = 0;
        while (counter < 3) {
            Block *tempBlock = this;
            bool shapeExists = false;
            while (tempBlock != nullptr) {
                if (tempBlock->shape == tempShape) {
                    shapeExists = true;
                    break;
                }
                tempBlock = tempBlock->right_rotation;
            }

            if (!shapeExists) {
                current->right_rotation = new Block;
                current->right_rotation->shape = tempShape;
                current->right_rotation->next_block = start->next_block;
                current->right_rotation->left_rotation = current;
                current = current->right_rotation;
            }

            vector<vector<bool>> newShape;
            for (int i = tempShape[0].size() - 1; i >= 0; --i) {
                vector<bool> newRow;
                newRow.reserve(tempShape.size());
                for (int j = 0; j < tempShape.size(); ++j) {
                    newRow.push_back(tempShape[tempShape.size() - 1 - j][i]);
                }
                newShape.push_back(newRow);
            }
            tempShape = newShape;
            counter++;
        }

        current->right_rotation = start;
        start->left_rotation = current;
    }


};


#endif //PA2_BLOCK_H
