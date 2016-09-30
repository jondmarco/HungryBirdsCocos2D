#ifndef GameScene_hpp
#define GameScene_hpp

#pragma once
#include "cocos2d.h"
#include "Larva.h"
#include "Bird.h"
#include "GameManager.h"

enum AIMovement{
	LARVAMOV,
	BIRDMOV1,
	BIRDMOV2,
	BIRDMOV3,
	BIRDMOV4
};

class GameState;

namespace HungryBirds{;

class Game : public cocos2d::Layer
{
public:
	virtual bool init() override;

	static cocos2d::Scene* scene();

	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*) override;
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*) override;
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*) override;
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*) override;

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

	Larva* getLarva() const;
	std::vector<Bird*> getBirds() const;

	bool larvaWon();
	bool birdsWon();
	void playTurn();
	void playTwoAi();
    
    void buttonPressedCallback(Ref* sender, int tag);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);
private:
	cocos2d::Label* m_turnLabel;
	Larva* larva;
	std::vector<Bird*> birdsVector;
	int clickedPosition1;
	int clickedPosition2;
	bool m_isGameFinished;
	bool m_turnHasBeenGenerated;
	AIMovement m_typeMov;

	void generateAITurn();
	void resetHighlights();
};
}
#endif /* GameScene_hpp */
