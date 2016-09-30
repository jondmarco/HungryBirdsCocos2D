#pragma once
#include "cocos2d.h"
#ifdef __APPLE__
#include "Map.h"
#else
#include "Classes\Map.h"
#endif

#include "AIPlayer.h"

class Larva : public AIPlayer
{
public:
	Larva();
	~Larva();

	int getPosition() const;
	cocos2d::Sprite* getSprite() const;
	void setPosition(int& position);
	
	std::vector<int> getCellsInRange();
	bool move(int& newPosition);

private:
	bool canMove(int& newPosition);
	bool atFirstColumn();
	bool atLastColumn();

	int position;
	cocos2d::Sprite* _sprite;
};