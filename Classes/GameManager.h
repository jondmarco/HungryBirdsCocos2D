#ifndef GameManager_hpp
#define GameManager_hpp

#include "cocos2d.h"
#include <vector>

enum GameType
{
	TWOPLAYERS = 0,
	AIVSAI = 1,
	AITOURNAMENT = 2
	
};

enum ChoosePLayerType
{
	LARVACHOSEN = 0,
	BIRDSCHOSEN = 1
};

typedef std::vector<int> PastMoves;

namespace HungryBirds{;

class GameManager
{
public:
    static GameManager* getInstance();
    
    /// @Returns true if the game is in the frist player's turn
    bool isFirstPlayerTurn();
    
    /// Changes the turn of the player
    void changeTurn();
    
    /// @params: The index of the tile in the last turn played
    /// Saves the player move on one of the vectors
    void saveMove(const int& playerMove);

	void resetTurns();

	GameType getGameType();
	void setGameType(GameType gType);

	ChoosePLayerType getPlayerType();
	void setPlayerType(ChoosePLayerType pType);
    
private:
    
    GameManager();
    ~GameManager();
    
    PastMoves m_firstPlayerMoves;
    PastMoves m_secondPlayerMoves;
    
    short m_playerTurn;
	GameType m_gameType;
	ChoosePLayerType m_playerType;
};
} // End of HungryBirds

#endif /* GameManager_hpp */
