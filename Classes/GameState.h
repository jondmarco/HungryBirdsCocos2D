#pragma once
#include "cocos2d.h"

enum PlayerType {LARVA = 0, BIRD1, BIRD2, BIRD3, BIRD4};
class GameState 
{
public:
	GameState(int larvaPosition, int b1Position, int b2Position, int b3Position, int b4Position);
	GameState();
	~GameState();
	std::vector<int> getGameStatePositions();
	int getLarvaPosition();
	int getBirdPosition(int birdNumber);
	std::vector<int> getBirdsPositions();
	std::vector<int> getPossibleMoves(PlayerType playerType);
	bool larvaWon();
	bool birdsWon();
private:
	std::vector<int> gameStatePositions; 
	std::vector<int> getLarvaPossibleMoves();
	std::vector<int> getBirdPossibleMoves(int birdNumber);
	bool atFirstColumn(int& position);
	bool atLastColumn(int& position);
	bool isOccupied(int& position);
};