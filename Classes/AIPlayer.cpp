#include "AIPlayer.h"

int AIPlayer::generatePositionForAi()
{
	return cocos2d::random() % 64;
}