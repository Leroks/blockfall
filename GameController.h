#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    static int downAmount(BlockFall& game,int x, int y);

    bool checkCollision(long pos, BlockFall &fall);

    unsigned long clearRow(BlockFall &fall, int i);

    void dropWithGravity(BlockFall &fall);

    void dropWithoutGravity(BlockFall &fall);

    int downAmountShape(BlockFall &fall);

    bool checkPowerUp(BlockFall &fall);

    void usePowerUp(BlockFall &fall);

    void printEmptyGrid(BlockFall &fall, bool b);

    void printEmptyGrid2(BlockFall &fall);

    int clear(BlockFall &fall);

    bool isRowFull(BlockFall &fall, int i);

    void changeGravity(BlockFall &game);

    void printGrid(BlockFall &game);
};


#endif //PA2_GAMECONTROLLER_H
