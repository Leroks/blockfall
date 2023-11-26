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

    static vector<vector<bool>> rotate90degrees(const vector<vector<bool>> &inputShapeGiven) {
        vector<vector<bool>> result;
        for (int i = inputShapeGiven[0].size() - 1; i >= 0; --i) {
            vector<bool> newRow;
            newRow.reserve(inputShapeGiven.size());
            for (int j = 0; j < inputShapeGiven.size(); ++j) {
                newRow.push_back(inputShapeGiven[inputShapeGiven.size() - 1 - j][i]);
            }
            result.push_back(newRow);
        }
        return result;
    }

    bool isShapeInList(const vector<vector<bool>> &shapeGiven) {
        Block *tempBlock = this;
        while (tempBlock != nullptr) {
            if (tempBlock->shape == shapeGiven) {
                return true;
            }
            tempBlock = tempBlock->right_rotation;
        }
        return false;
    }


    void createCircularLinkedList() {
        Block *start = this;
        Block *current = start;
        vector<vector<bool>> tempShape = rotate90degrees(start->shape);

        for (int i = 0; i < 3; ++i) {
            if (!isShapeInList(tempShape)) {
                current->right_rotation = new Block;
                current->right_rotation->shape = tempShape;
                current->right_rotation->next_block = start->next_block;
                current->right_rotation->left_rotation = current;
                current = current->right_rotation;
            }
            tempShape = rotate90degrees(tempShape);
        }

        current->right_rotation = start;
        start->left_rotation = current;
    }

};


#endif //PA2_BLOCK_H
