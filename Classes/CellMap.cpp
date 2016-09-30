#include "CellMap.h"

USING_NS_CC;

CellMap::CellMap()
	: m_cellSprite(Sprite::create("Cell.png")), occupied(false)
{
    
}

CellMap::~CellMap()
{
    
}

void CellMap::setPosition(const cocos2d::Vec2& position)
{
    m_cellSprite->setPosition(position);
}

cocos2d::Vec2 CellMap::getPosition()
{
    return m_cellSprite->getPosition();
}

cocos2d::Sprite* CellMap::getSprite()
{
    return m_cellSprite;
}

void CellMap::highlightSprite()
{
	if (m_cellSprite->getColor() == Color3B(255, 255, 255))
	{
		m_cellSprite->setColor(Color3B(240, 240, 0));
	}
	else
	{
		m_cellSprite->setColor(Color3B(255, 255, 255));
	}
}

bool CellMap::isCellClicked(const cocos2d::Vec2& clickPos) const
{
    return m_cellSprite->getBoundingBox().containsPoint(clickPos);
}

bool CellMap::isOccupied() const
{
	return occupied;
}

void CellMap::setIsOccupied(bool occupied)
{
	this->occupied = occupied;
}