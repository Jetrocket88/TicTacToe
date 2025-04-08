#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <vector>
#include <fstream>
#include <sstream>

#include "SpriteRenderer.h"
#include "GameObject.h"
#include "ResourceManager.h"

class GameLevel
{
public:
    // level state
    std::vector<GameObject> Squares;
    // constructor
    GameLevel() {}
    // loads level from file
    // render level
    void Draw(SpriteRenderer& renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
private:
    // initialize level from tile data
    void init();
};
#endif // !GAME_LEVEL_H


