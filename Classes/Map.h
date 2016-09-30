#ifndef Map_hpp
#define Map_hpp

#include "cocos2d.h"
#include <vector>

// forward declaration
class CellMap;

typedef std::vector<CellMap> MapVector;

namespace HungryBirds{;

class Map
{
public:
    static Map* getInstance();
    
    //setter and getters
    void setPositionOfTheMapAndDrawThemOnTheScene(const cocos2d::Vec2& position, cocos2d::Layer* drawingLayer);

    ///  @returns: True if a click intersects a tile
    ///  @params: The position in the layer where the click occured
    bool isClickIntersectingATile(const cocos2d::Vec2& clickPos);
    
    /// @returns: The position in the view for a tile. If the does not exist, it returns NULL
    /// @params: The index in the vector for the tile.
    cocos2d::Vec2 getPositionForCellAtIndex(const int& index);
    
    /// @returns: The index in the vector for a tile.
    /// @params: The position where the tile is located at in the view.
    int getIndexOfCellWithPosition(const cocos2d::Vec2& position);

	bool isMapCellOccupied(int& position);
	void setIsMapCellOccupied(bool occupied, int& position);
	void highlightCell(int& position);
    
private:
    Map();
    ~Map();
    MapVector m_mapVector;

	void createMap();
    void printReferenceLabelsOnCellMap(const int n, const cocos2d::Vec2& position ,cocos2d::Layer* drawingLayer);

};
} // End of HungryBirds

#endif /* Map_hpp */
