#include "AppMacros.h"
#include "ChoosePlayerScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;
using namespace HungryBirds;

Scene* MainMenu::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    MainMenu *layer = MainMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create("MainMenuWall.jpg");
    background->setScaleX((visibleSize.width / background->getContentSize().width) * 1);
    background->setScaleY((visibleSize.height / background->getContentSize().height) * 1);

    background->setPosition(Vec2(visibleSize / 2) + origin);
    this->addChild(background, 1);
    
    auto oneAiPlayer = MenuItemLabel::create(Label::createWithSystemFont("AI Tournamet",
                                                                         "Arial",
                                                                         TITLE_FONT_SIZE),
                                             CC_CALLBACK_1(MainMenu::buttonPressedCallback, 
															this, 
															GameType::AITOURNAMENT));
    
    oneAiPlayer->setPosition(0,0);
    oneAiPlayer->getLabel()->setColor(Color3B(0, 0, 0));
    
    auto twoAiPlayerItem = MenuItemLabel::create(Label::createWithSystemFont("AI vs AI",
                                                                             "Arial",
                                                                             TITLE_FONT_SIZE),
                                                 CC_CALLBACK_1(MainMenu::buttonPressedCallback, 
																this, 
																GameType::AIVSAI));
    
    twoAiPlayerItem->setPosition(0, oneAiPlayer->getPosition().y + oneAiPlayer->getBoundingBox().size.height);
    twoAiPlayerItem->getLabel()->setColor(Color3B(0, 0, 0));
    
    auto twoPlayerItem = MenuItemLabel::create(Label::createWithSystemFont("1 vs 1",
                                                                           "Arial",
                                                                           TITLE_FONT_SIZE),
                                               CC_CALLBACK_1(MainMenu::buttonPressedCallback, 
															this, 
															GameType::TWOPLAYERS));
    
    twoPlayerItem->setPosition(0,twoAiPlayerItem->getPosition().y + twoAiPlayerItem->getBoundingBox().size.height);
    twoPlayerItem->getLabel()->setColor(Color3B(0, 0, 0));
    
    
    auto menu = Menu::create(twoPlayerItem, twoAiPlayerItem, oneAiPlayer, nullptr);
    menu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 92);
    this->addChild(menu, 1);
    
    
    return true;
}

void MainMenu::buttonPressedCallback(Ref* sender, int tag)
{
	GameType gtype = static_cast<GameType>(tag);
	GameManager::getInstance()->setGameType(gtype);
	if(gtype != GameType::AITOURNAMENT)
	{
		Director::getInstance()->replaceScene(HungryBirds::Game::scene());
	}
	else
	{
		Director::getInstance()->replaceScene(ChoosePLayer::scene());
	}
}
