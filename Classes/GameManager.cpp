#include "GameManager.h"

USING_NS_CC;

namespace HungryBirds {;

static GameManager* s_instance = nullptr;
    
GameManager* GameManager::getInstance()
{
    if(!s_instance)
    {
        s_instance =  new GameManager();
    }
    return s_instance;
}
    
GameManager::GameManager()
:m_playerTurn(1)
{
        
}
    
GameManager::~GameManager()
{
    m_secondPlayerMoves.clear();
    m_firstPlayerMoves.clear();
}
    
bool GameManager::isFirstPlayerTurn()
{
    return m_playerTurn % 2;
}
    
void GameManager::changeTurn()
{
    m_playerTurn = (m_playerTurn + 1) % 2;
}
    
void GameManager::saveMove(const int& playerMove)
{
    if(m_playerTurn)
    {
        m_firstPlayerMoves.push_back(playerMove);
    }
    else
    {
        m_secondPlayerMoves.push_back(playerMove);
    }
}

void GameManager::resetTurns()
{
	m_playerTurn = 1;
}

GameType GameManager::getGameType()
{
	return m_gameType;
}

void GameManager::setGameType(GameType gType)
{
	m_gameType = gType;
}

ChoosePLayerType GameManager::getPlayerType()
{
	return m_playerType;
}

void GameManager::setPlayerType(ChoosePLayerType pType)
{
	m_playerType = pType;
}
    
} // End of HungryBirds