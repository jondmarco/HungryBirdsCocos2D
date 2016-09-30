
#include "Larva.h"

USING_NS_CC;

#define INITIAL_POSITION 51;

Larva::Larva()
{
	this->position = INITIAL_POSITION;
	_sprite = Sprite::create("larva.png");
	this->_sprite->setPosition(HungryBirds::Map::getInstance()->getPositionForCellAtIndex(position));
	HungryBirds::Map::getInstance()->setIsMapCellOccupied(true, this->position);
}

Larva::~Larva()
{

}

int Larva::getPosition() const
{
	return position;
}

void Larva::setPosition(int& position)
{
	this->position = position;
}

cocos2d::Sprite* Larva::getSprite() const
{
	return _sprite;
}

std::vector<int> Larva::getCellsInRange()
{
	std::vector<int> positionsOfCellsInRange;

	int topLeft = position - 9;
	int bottomLeft = position + 7;
	int topRight = position - 7;
	int bottomRight = position + 9;

	if (!atFirstColumn())
	{
		if ((topLeft >= 0) && (topLeft < 64) && (!HungryBirds::Map::getInstance()->isMapCellOccupied(topLeft)))
		{
			positionsOfCellsInRange.push_back(topLeft);
		}

		if ((bottomLeft >= 0) && (bottomLeft < 64) && (!HungryBirds::Map::getInstance()->isMapCellOccupied(bottomLeft)))
		{
			positionsOfCellsInRange.push_back(bottomLeft);
		}
	}
	
	if (!atLastColumn())
	{
		if ((topRight >= 0) && (topRight < 64) && (!HungryBirds::Map::getInstance()->isMapCellOccupied(topRight)))
		{
			positionsOfCellsInRange.push_back(topRight);
		}

		if ((bottomRight >= 0) && (bottomRight < 64) && (!HungryBirds::Map::getInstance()->isMapCellOccupied(bottomRight)))
		{
			positionsOfCellsInRange.push_back(bottomRight);
		}
	}

	return positionsOfCellsInRange;
}

bool Larva::canMove(int& newPosition) 
{ 
	//at last column cant go top right or bottom right 
	if (atLastColumn() && ((position - newPosition) == 7 || (position - newPosition) == -9))
	{
		return false;
	}
	//at first column cant go top left or bottom left
	else if (atFirstColumn() && ((position - newPosition) == 9 || (position - newPosition) == -7))
	{
		return false;
	}
	else if (((abs(position - newPosition)) == 7 || (abs(position - newPosition) == 9))
			&& !HungryBirds::Map::getInstance()->isMapCellOccupied(newPosition))
	{
		return true;
	}
	return false;
}

bool Larva::move(int& newPosition) 
{
	//move the larva to a given point
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

bool Larva::atFirstColumn()
{
	return (this->position % 8 == 0);
}

bool Larva::atLastColumn()
{
	return (this->position % 8 == 7);
}


