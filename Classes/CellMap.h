#ifndef CellMap_hpp
#define CellMap_hpp

#include "cocos2d.h"

class CellMap
{
public:
    CellMap();
    ~CellMap();
    
    //setters and getters
    void setPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 getPosition();
    cocos2d::Sprite* getSprite();
	bool isOccupied() const;
	void setIsOccupied(bool occupied);
    
    void highlightSprite();
    bool isCellClicked(const cocos2d::Vec2& clickPos) const;	
    
private:
    cocos2d::Sprite* m_cellSprite;
	bool occupied;
};

#endif /* CellMap_hpp */
