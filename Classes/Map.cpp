#include "Map.h"
#include "CellMap.h"

USING_NS_CC;

#define MAX_TILES 64
#define MAX_COLUM_SIZE 8

namespace HungryBirds{;
    
static Map* s_instance =  nullptr;

Map* Map::getInstance()
{
    if(!s_instance)
    {
        s_instance = new Map();
    }
    return s_instance;
}

Map::Map()
{
}

void Map::createMap()
{
	m_mapVector.clear();
	for (int i = 0; i < MAX_TILES; i++)
	{
		m_mapVector.push_back(CellMap());
	}
}

Map::~Map()
{
    m_mapVector.clear();
}

void Map::setPositionOfTheMapAndDrawThemOnTheScene(const cocos2d::Vec2& position, cocos2d::Layer* drawingLayer)
{
	createMap();
    int count = 0;
    Vec2 lastPosition = position;
    for(MapVector::iterator it = m_mapVector.begin(); it != m_mapVector.end(); ++it)
    {
        it->setPosition(lastPosition);
        Vec2 positionForLabel = lastPosition;
        count++;
        if(count % MAX_COLUM_SIZE != 0 )
        {
            lastPosition = Vec2(lastPosition.x + it->getSprite()->getBoundingBox().size.width, lastPosition.y);
        }
        else
        {
            lastPosition = Vec2(position.x, lastPosition.y - it->getSprite()->getBoundingBox().size.height);
        }
        
        drawingLayer->addChild(it->getSprite());
        // This and positionForLabel are for debugging purposes, they should not be included in the
        // final build.
        printReferenceLabelsOnCellMap(count-1, positionForLabel, drawingLayer);
        
        
        
    }
}

bool Map::isMapCellOccupied(int& position)
{
	return m_mapVector.at(position).isOccupied();
}

void Map::setIsMapCellOccupied(bool occupied,int& position)
{
	m_mapVector.at(position).setIsOccupied(occupied);
}


bool Map::isClickIntersectingATile(const cocos2d::Vec2& clickPos)
{
    for(MapVector::iterator it = m_mapVector.begin(); it != m_mapVector.end(); ++it)
    {
        if(it->isCellClicked(clickPos))
        {
            //For debugging purposes
            //it->highlightSprite();
            return true;
        }
    }
    return false;
}
    
int Map::getIndexOfCellWithPosition(const cocos2d::Vec2& position)
{
    for(int i = 0; i < MAX_TILES; i++)
    {
        if(m_mapVector.at(i).isCellClicked(position))
        {
            return i;
        }
    }
    return -1;
}
    
cocos2d::Vec2 Map::getPositionForCellAtIndex(const int& index)
{
    try {
        return m_mapVector.at(index).getPosition();
    } catch (const std::out_of_range& oor) {
        CCLOG("The index in the vector is out of bounds");
        return NULL;
    }
}

void Map::highlightCell(int& position)
{
	m_mapVector.at(position).highlightSprite();
}
    
void Map::printReferenceLabelsOnCellMap(const int n, const cocos2d::Vec2& position ,cocos2d::Layer* drawingLayer)
{
    std::stringstream s;
    s << n;
    cocos2d::Label *label = cocos2d::Label::createWithSystemFont(s.str(), "arial", 10);
    label->setPosition(position);
    label->setColor(Color3B(0,0,0));
    drawingLayer->addChild(label);
}
    
} // End of HungryBirds
