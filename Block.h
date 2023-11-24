#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>
#include <iostream>

using namespace std;

class Block {
public:
    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block *right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block *left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block *next_block = nullptr; // Pointer to the next block to appear in the game

    bool operator==(const Block &other) const {
        // TODO: Overload the == operator to compare two blocks based on their shapes
        if (shape.size() != other.shape.size() || shape[0].size() != other.shape[0].size()) {
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
        return !(*this == other);
    }

    vector<vector<bool>> rotate90Degrees(const vector<vector<bool>> &shapeToRotate) {
        vector<vector<bool>> rotatedShape;

        for (int i = shapeToRotate[0].size() - 1; i >= 0; i--) {
            vector<bool> row;
            for (int j = 0; j < shapeToRotate.size(); j++) {
                row.push_back(shapeToRotate[j][shapeToRotate[0].size() - 1 - i]);
            }
            rotatedShape.push_back(row);
        }

        return rotatedShape;
    }

    void fillTheCircular() {
        Block *head = this;
        Block *current = head;
        vector<vector<bool>> rotatedShape = rotate90Degrees(head->shape);

        for (int i = 0; i < 3; i++) {
            if (!isShapeInDoubly(rotatedShape)) {
                current->right_rotation = new Block;
                current->right_rotation->shape = rotatedShape;
                current->right_rotation->next_block = head->next_block;
                current->right_rotation->left_rotation = current;
                current = current->right_rotation;
            }
            rotatedShape = rotate90Degrees(rotatedShape);
        }

        current->right_rotation = head;
        head->left_rotation = current;
    }

private:
    bool isShapeInDoubly(const vector<vector<bool>> &shapeToCheck) {
        // Only works while building the circular doubly linked list
        Block *tmp = this;
        while (tmp != nullptr) {
            if (tmp->shape == shapeToCheck) {
                return true;
            }
            tmp = tmp->right_rotation;
        }
        return false;
    }
};

#endif //PA2_BLOCK_H
