#include "AppMacros.h"
#include "GameScene.h"
#include "GameState.h"
#include "MainMenuScene.h"
#include "Map.h"
#include "MiniMax.h"

#define PLAYER_ONE_TEXT "Larva"
#define PLAYER_TWO_TEXT "Birds"
#define TURN_LABEL_Y 30
#define LARVA_WON_TEXT "Larva wins the game!"
#define BIRDS_WON_TEXT "Birds won the game!"
#define SEARCH_DEPTH 4;
using namespace HungryBirds;

cocos2d::Scene* Game::scene()
{
    // 'scene' is an autorelease object
	auto scene = cocos2d::Scene::create();
    
    // 'layer' is an autorelease object
    Game *layer = Game::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool HungryBirds::Game::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	m_isGameFinished = false;
    
	auto background = cocos2d::Sprite::create("GameWall.jpg");
    background->setScaleX((visibleSize.width / background->getContentSize().width) * 1);
    background->setScaleY((visibleSize.height / background->getContentSize().height) * 1);
    
	background->setPosition(cocos2d::Vec2(visibleSize / 2) + origin);
    this->addChild(background, -1);
    
	m_turnLabel = cocos2d::Label::createWithSystemFont(PLAYER_ONE_TEXT, "ARIAL", TITLE_FONT_SIZE);
    m_turnLabel->setPosition(visibleSize.width / 2, TURN_LABEL_Y);
	m_turnLabel->setColor(cocos2d::Color3B(0, 0, 0));
    this->addChild(m_turnLabel);
    
	HungryBirds::Map::getInstance()->setPositionOfTheMapAndDrawThemOnTheScene(cocos2d::Vec2(visibleSize.width / 3.5, visibleSize.height / 1.2)
	                                                                           ,this);
    
    // Button menu
    // This part is commented because neither Map or GameManager should be singleton
    // Because they are singleton everytime the menu loads this scene the singleont objects
    // which are already created, get created again and this craashes the program
    // TODO: uncomment and fix those 2 classes.
    auto BackBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("BackBtn.png"),
                                                     cocos2d::Sprite::create("BackBtn_.png"),
                                                     CC_CALLBACK_1(Game::buttonPressedCallback, this, 0));
    
    auto restartBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("RestartBtn.png"),
                                                     cocos2d::Sprite::create("RestartBtn_.png"),
                                                     CC_CALLBACK_1(Game::buttonPressedCallback, this, 1));
    restartBtn->setPosition(BackBtn->getPosition().x,
                            BackBtn->getPosition().y - restartBtn->getBoundingBox().size.height - 20);
    
    auto menu = cocos2d::Menu::create(BackBtn, restartBtn,NULL);
    menu->setPosition(visibleSize.width - restartBtn->boundingBox().size.width * 2,
                      visibleSize.height - restartBtn->boundingBox().size.height);
    this->addChild(menu);
    
	//add larva to scene
	larva = new Larva();
	this->addChild(larva->getSprite());

	//add 4 birds to scene
	birdsVector.push_back(new Bird(1));
	birdsVector.push_back(new Bird(2));
	birdsVector.push_back(new Bird(3));
	birdsVector.push_back(new Bird(4));
	for (int i = 0; i < 4; ++i)
	{
		this->addChild(birdsVector.at(i)->getSprite());
	}

	if(GameManager::getInstance()->getGameType() != GameType::AIVSAI)
	{
		//Setup the touch/click events and event listener
		auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    
		touchListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(Game::onTouchCancelled, this);
    
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	}

	//Setup the keyboard listener
	auto keyboardListener = cocos2d::EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	//initial value -- nothing is clicked
	clickedPosition1 = -1;
	clickedPosition2 = -1;

	//reset turns to default
	GameManager::getInstance()->resetTurns();
	m_turnHasBeenGenerated = false;
    return true;
}

bool Game::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	// You should be able to click on the grid if the game is two vs two or
	// if it is one vs ai, but if you are the larva and it is the first player turn
	// or if you are the birds, but it is not the first player turn.
	if((GameManager::getInstance()->getPlayerType() == ChoosePLayerType::LARVACHOSEN && 
		GameManager::getInstance()->isFirstPlayerTurn()) ||
		(GameManager::getInstance()->getPlayerType() == ChoosePLayerType::BIRDSCHOSEN &&
		!GameManager::getInstance()->isFirstPlayerTurn()) || 
		GameManager::getInstance()->getGameType() == GameType::TWOPLAYERS)
	{

		cocos2d::Vec2 location = touch->getLocationInView();
		location = cocos2d::Director::getInstance()->convertToGL(location);

		//there can only be two clicked cells on the map at a time
		if (HungryBirds::Map::getInstance()->isClickIntersectingATile(location))
		{
			int clickedCellPosition = HungryBirds::Map::getInstance()->getIndexOfCellWithPosition(location);

			if (clickedPosition1 != clickedCellPosition && clickedPosition2 != clickedCellPosition && (clickedPosition1 == -1 || clickedPosition2 == -1))
			{
				if (clickedPosition1 == -1)
				{
					clickedPosition1 = clickedCellPosition;
					HungryBirds::Map::getInstance()->highlightCell(clickedPosition1);
				}
				else
				{
					clickedPosition2 = clickedCellPosition;
					HungryBirds::Map::getInstance()->highlightCell(clickedPosition2);
				}
			}
			else if (clickedPosition1 == clickedCellPosition)
			{
				clickedPosition1 = -1;
				HungryBirds::Map::getInstance()->highlightCell(clickedCellPosition);
			}
			else if (clickedPosition2 == clickedCellPosition)
			{
				clickedPosition2 = -1;
				HungryBirds::Map::getInstance()->highlightCell(clickedCellPosition);
			}
		}
	}

    return true;
}

void Game::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    cocos2d::log("touch ended");
}

void Game::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    cocos2d::log("touch moved");
}

void Game::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
    cocos2d::log("touch cancelled");
}

void Game::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
	{
		cocos2d::log("key SPACE pressed");
	}
}

void Game::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	// If the key is enter and the ai movement has not been generated you can go in
	if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER && !m_turnHasBeenGenerated)
	{
		// If the game type is AIVSAI you can generate a turn
		// If the game is Player vs AI and it is not the human being turn, then you should be
		// able to generate a turn
		if(GameManager::getInstance()->getGameType() == GameType::AIVSAI ||
		(GameManager::getInstance()->getGameType() == GameType::AITOURNAMENT &&
		((GameManager::getInstance()->getPlayerType() != ChoosePLayerType::LARVACHOSEN && 
		GameManager::getInstance()->isFirstPlayerTurn()) ||
		(GameManager::getInstance()->getPlayerType() != ChoosePLayerType::BIRDSCHOSEN &&
		!GameManager::getInstance()->isFirstPlayerTurn()))))
		{
			generateAITurn();
		}
	}
	// If the keycode is space and the positions are valid and the type of game is 2 v 2, then play
	// the human being turn
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE && !m_isGameFinished && 
		GameManager::getInstance()->getGameType() == GameType::TWOPLAYERS && 
		clickedPosition1 != -1 && clickedPosition2 != -1)
	{
 		playTurn();
		resetHighlights();
	}
	// If the game is AI vs AI and the turn has been generated then play the AI movement
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE && !m_isGameFinished && 
		GameManager::getInstance()->getGameType() != GameType::TWOPLAYERS && m_turnHasBeenGenerated)
	{
 		playTwoAi();
		resetHighlights();
	}
	// If the game is type Players vs AI and it is the human being turn and the tiles he/she selected
	// are valid
	else if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE && !m_isGameFinished && 
		GameManager::getInstance()->getGameType() == GameType::AITOURNAMENT && 
		clickedPosition1 != -1 && clickedPosition2 != -1)
	{
		// If it is the first player turn and the human being picked larva or
		// If it is not the first player turn and the human being picked birds, then
		// We should play the human being turn
		if((GameManager::getInstance()->getPlayerType() == ChoosePLayerType::LARVACHOSEN && 
		GameManager::getInstance()->isFirstPlayerTurn()) ||
		(GameManager::getInstance()->getPlayerType() == ChoosePLayerType::BIRDSCHOSEN &&
		!GameManager::getInstance()->isFirstPlayerTurn()))
		{
			playTurn();
			resetHighlights();
		}
	}
}

void Game::playTwoAi()
{
	switch (m_typeMov)
	{
	case LARVAMOV: larva->move(clickedPosition2);
		break;
	case BIRDMOV1: birdsVector.at(0)->move(clickedPosition2);
		break;
	case BIRDMOV2: birdsVector.at(1)->move(clickedPosition2);
		break;
	case BIRDMOV3: birdsVector.at(2)->move(clickedPosition2);
		break;
	case BIRDMOV4: birdsVector.at(3)->move(clickedPosition2);
		break;
	default:
		break;
	}
	
	m_turnHasBeenGenerated = false;

	if (larvaWon())
	{
		//TODO: change scene?

		//temporary for testing
		m_turnLabel->setString(LARVA_WON_TEXT);
		m_isGameFinished = true;
	}
	else if (birdsWon())
	{
		//TODO: change scene?

		//temporary for testing
		m_turnLabel->setString(BIRDS_WON_TEXT);
		m_isGameFinished = true;
	}
	else
	{
		GameManager::getInstance()->changeTurn();
		if (GameManager::getInstance()->isFirstPlayerTurn())
		{
			m_turnLabel->setString(PLAYER_ONE_TEXT);
		}
		else
		{
			m_turnLabel->setString(PLAYER_TWO_TEXT);
		}
	}
}

void Game::playTurn()
{
	//Larva's turn
	if (GameManager::getInstance()->isFirstPlayerTurn())
	{
		if (clickedPosition1 == larva->getPosition() && larva->move(clickedPosition2))
		{
			if (larvaWon())
			{
				//TODO: change scene?

				//temporary for testing
				m_turnLabel->setString(LARVA_WON_TEXT);
				m_isGameFinished = true;
			}
			else if (birdsWon())
			{
				//TODO: change scene?

				//temporary for testing
				m_turnLabel->setString(BIRDS_WON_TEXT);
				m_isGameFinished = true;
			}
			else
			{
				GameManager::getInstance()->changeTurn();
				m_turnLabel->setString(PLAYER_TWO_TEXT);
			}
		}
		else if (clickedPosition2 == larva->getPosition() && larva->move(clickedPosition1))
		{
			if (larvaWon())
			{
				//TODO: change scene?

				//temporary for testing
				m_turnLabel->setString(LARVA_WON_TEXT);
				m_isGameFinished = true;
			}
			else if (birdsWon())
			{
				//TODO: change scene?

				//temporary for testing
				m_turnLabel->setString(BIRDS_WON_TEXT);
				m_isGameFinished = true;
			}
			else
			{
				GameManager::getInstance()->changeTurn();
				m_turnLabel->setString(PLAYER_TWO_TEXT);
			}
		}
	}

	//Birds turn
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			if (clickedPosition1 == birdsVector.at(i)->getPosition() && birdsVector.at(i)->move(clickedPosition2))
			{
				if (larvaWon())
				{
					//TODO: change scene?

					//temporary for testing
					m_turnLabel->setString(LARVA_WON_TEXT);
					m_isGameFinished = true;
				}
				else if (birdsWon())
				{
					//TODO: change scene?

					//temporary for testing
					m_turnLabel->setString(BIRDS_WON_TEXT);
					m_isGameFinished = true;
				}
				else
				{
					GameManager::getInstance()->changeTurn();
					m_turnLabel->setString(PLAYER_ONE_TEXT);
				}
			}
			else if (clickedPosition2 == birdsVector.at(i)->getPosition() && birdsVector.at(i)->move(clickedPosition1))
			{
				if (larvaWon())
				{
					//TODO: change scene?

					//temporary for testing
					m_turnLabel->setString(LARVA_WON_TEXT);
					m_isGameFinished = true;
				}
				else if (birdsWon())
				{
					//TODO: change scene?

					//temporary for testing
					m_turnLabel->setString(BIRDS_WON_TEXT);
					m_isGameFinished = true;
				}
				else
				{
					GameManager::getInstance()->changeTurn();
					m_turnLabel->setString(PLAYER_ONE_TEXT);
				}
			}
		}
	}
}

void Game::buttonPressedCallback(Ref* sender, int tag)
{
    if(tag == 0)
    {
        cocos2d::Director::getInstance()->replaceScene(MainMenu::scene());
    }
    else
    {
        cocos2d::Director::getInstance()->replaceScene(Game::scene());
    }
}

bool Game::larvaWon()
{
	int birdsNotAvailable = 0;
	for(int i = 0; i < 4; ++i)
	{
		//at last row
		if (birdsVector.at(i)->getPosition() >= 0 && birdsVector.at(i)->getPosition() <= 7)
		{
			birdsNotAvailable++;
		}
		else if (birdsVector.at(i)->getCellsInRange().size() == 0)
		{ 
			birdsNotAvailable++;
		}
	}
	return ((larva->getPosition() >= 56 && larva->getPosition() <= 63) || birdsNotAvailable == 4);
}

bool Game::birdsWon()
{
	return (larva->getCellsInRange().size() == 0);
}

Larva* Game::getLarva() const
{
	return larva;
}

std::vector<Bird*> Game::getBirds() const
{
	return birdsVector;
}

void Game::generateAITurn()
{

	GameNode::reset();
	GameState* currentGameState = new GameState(larva->getPosition(), 
												birdsVector.at(0)->getPosition(), 
												birdsVector.at(1)->getPosition(),
											    birdsVector.at(2)->getPosition(), 
												birdsVector.at(3)->getPosition());	

	//depth of the tree; top level is 0
	int depth = SEARCH_DEPTH;
	GameState* gameState = MiniMax::findBestPossibleMove(currentGameState, depth, GameManager::getInstance()->isFirstPlayerTurn());
	m_turnHasBeenGenerated = true;
	//check if larva moved or which bird moved and update game accordingly
	if (gameState->getLarvaPosition() != larva->getPosition())
	{
		clickedPosition1 = larva->getPosition();
		clickedPosition2 = gameState->getLarvaPosition();
		m_typeMov = AIMovement::LARVAMOV;
	}
	else if (gameState->getBirdPosition(1) != birdsVector.at(0)->getPosition())
	{
		clickedPosition1 = birdsVector.at(0)->getPosition();
		clickedPosition2 = gameState->getBirdPosition(1);
		m_typeMov = AIMovement::BIRDMOV1;
	}
	else if (gameState->getBirdPosition(2) != birdsVector.at(1)->getPosition())
	{
		clickedPosition1 = birdsVector.at(1)->getPosition();
		clickedPosition2 = gameState->getBirdPosition(2);
		m_typeMov = AIMovement::BIRDMOV2;
	}
	else if (gameState->getBirdPosition(3) != birdsVector.at(2)->getPosition())
	{
		clickedPosition1 = birdsVector.at(2)->getPosition();
		clickedPosition2 = gameState->getBirdPosition(3);
		m_typeMov = AIMovement::BIRDMOV3;
	}
	else if (gameState->getBirdPosition(4) != birdsVector.at(3)->getPosition())
	{
		clickedPosition1 = birdsVector.at(3)->getPosition();
		clickedPosition2 = gameState->getBirdPosition(4);
		m_typeMov = AIMovement::BIRDMOV4;
	}
	
	HungryBirds::Map::getInstance()->highlightCell(clickedPosition1);
	HungryBirds::Map::getInstance()->highlightCell(clickedPosition2);
}

void Game::resetHighlights()
{
	HungryBirds::Map::getInstance()->highlightCell(clickedPosition1);
	HungryBirds::Map::getInstance()->highlightCell(clickedPosition2);
	clickedPosition1 = -1;
	clickedPosition2 = -1;
}