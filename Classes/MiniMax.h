#pragma once
#include "GameNode.h"

class MiniMax
{
public:
	static GameState* findBestPossibleMove(GameState* currentGameState, int& depth, bool turn);
	static GameState* execute(std::vector<int> currentLevelIndexes, bool turn);
};