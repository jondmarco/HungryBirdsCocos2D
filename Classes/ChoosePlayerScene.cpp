#include "AppMacros.h"
#include "ChoosePlayerScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;
using namespace HungryBirds;

Scene* ChoosePLayer::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    ChoosePLayer *layer = ChoosePLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool ChoosePLayer::init()
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

	auto titleLbl = cocos2d::Label::createWithSystemFont("Choose your character:", "ARIAL", TITLE_FONT_SIZE * 2);
	titleLbl->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 20));
	titleLbl->setColor(cocos2d::Color3B(0, 0, 0));
	this->addChild(titleLbl, 1);

	auto normalLarva = Sprite::create("larva.png");
	auto highlightedLarva = Sprite::create("larva.png");
	highlightedLarva->setColor(Color3B(240, 240, 0));
	auto larvaItem = MenuItemSprite::create(normalLarva, highlightedLarva, NULL,
											CC_CALLBACK_1(ChoosePLayer::buttonPressedCallback,
															this,
															ChoosePLayerType::LARVACHOSEN));
	larvaItem->setScale(2);
	larvaItem->setPosition(-100, 120);

	auto normalBird = Sprite::create("bird.png");
	auto highlightedBird = Sprite::create("bird.png");
	highlightedBird->setColor(Color3B(240, 240, 0));
	auto birdItem = MenuItemSprite::create(normalBird, highlightedBird, NULL,
											CC_CALLBACK_1(ChoosePLayer::buttonPressedCallback,
															this,
															ChoosePLayerType::BIRDSCHOSEN));
	birdItem->setScale(2);
	birdItem->setPosition(100, 120);
    
    
    auto menu = Menu::create(larvaItem, birdItem, nullptr);
    menu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 92);
    this->addChild(menu, 1);
    
    
    return true;
}

void ChoosePLayer::buttonPressedCallback(Ref* sender, int tag)
{
	ChoosePLayerType ptype = static_cast<ChoosePLayerType>(tag);
	GameManager::getInstance()->setPlayerType(ptype);
	Director::getInstance()->replaceScene(HungryBirds::Game::scene());
}