#include "../headers/GameLevel.h"


void GameLevel::Draw(SpriteRenderer& renderer)
{
    for (GameObject& tile : this->Squares)
		tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
	return false;
}

void GameLevel::init()
{

}
