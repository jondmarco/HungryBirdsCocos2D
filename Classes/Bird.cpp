#include "Bird.h"

USING_NS_CC;

#define INITIAL_POSITION_BIRD1 56;
#define INITIAL_POSITION_BIRD2 58;
#define INITIAL_POSITION_BIRD3 60;
#define INITIAL_POSITION_BIRD4 62;

Bird::Bird(int birdNumber)
{
	switch (birdNumber)
	{
	case 1:
		this->position = INITIAL_POSITION_BIRD1;
		break;
	case 2:
		this->position = INITIAL_POSITION_BIRD2;
		break;
	case 3:
		this->position = INITIAL_POSITION_BIRD3;
		break;
	case 4:
		this->position = INITIAL_POSITION_BIRD4;
		break;
	}	
	
	_sprite = Sprite::create("bird.png");
	_sprite->setPosition(HungryBirds::Map::getInstance()->getPositionForCellAtIndex(position));
	HungryBirds::Map::getInstance()->setIsMapCellOccupied(true, this->position);
}

Bird::~Bird()
{

}

int Bird::getPosition() const
{
	return position;
}

void Bird::setPosition(int& position)
{
	this->position = position;
}

cocos2d::Sprite* Bird::getSprite() const
{
	return _sprite;
}

std::vector<int> Bird::getCellsInRange()
{
	std::vector<int> positionsOfCellsInRange;

	//at last row; position 0 to 7
	if (position >= 0 && position <= 7)
	{
		return positionsOfCellsInRange;
	}

	int topLeft = position - 9;
	int topRight = position - 7;

	if (!atFirstColumn() && (topLeft >= 0) && (topLeft < 64) && (!HungryBirds::Map::getInstance()->isMapCellOccupied(topLeft)))
	{
		positionsOfCellsInRange.push_back(topLeft);
	}

	if (!atLastColumn() && (topRight >= 0) && (topRight < 64) && (!HungryBirds::Map::getInstance()->isMapCellOccupied(topRight)))
	{
		positionsOfCellsInRange.push_back(topRight);
	}

	return positionsOfCellsInRange;
}

bool Bird::canMove(int& newPosition)
{
	//can only move top left or top right
	if ((((position - newPosition) == 7 && !atLastColumn()) || ((position - newPosition) == 9 && !atFirstColumn()))
		&& !HungryBirds::Map::getInstance()->isMapCellOccupied(newPosition))
	{
		return true;
	}
	return false;
}

bool Bird::move(int& newPosition)
{
	//move the Bird to a given point
	if (canMove(newPosition))
	{
		HungryBirds::Map::getInstance()->setIsMapCellOccupied(false, this->position);
		this->position = newPosition;
		HungryBirds::Map::getInstance()->setIsMapCellOccupied(true, this->position);
		Vec2 point = HungryBirds::Map::getInstance()->getPositionForCellAtIndex(newPosition);
		auto move = MoveTo::create(1, point);
		this->getSprite()->runAction(move);
		return true;
	}
	return false;
}

bool Bird::atFirstColumn()
{
	return (this->position % 8 == 0);
}

bool Bird::atLastColumn()
{
	return (this->position % 8 == 7);
}


