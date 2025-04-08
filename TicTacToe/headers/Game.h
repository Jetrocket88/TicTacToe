#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "ResourceManager.h"


class Game
{
    public:
        // game state
        unsigned int Width, Height;
        bool Keys[1024];
        std::vector<GameObject> Squares;
        int turn = 0;

        // constructor/destructor
        Game(unsigned int width, unsigned int height);
        ~Game();
        // initialize game state (load all shaders/textures/levels)
        void Init();
        // game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
private:
};

#endif 
