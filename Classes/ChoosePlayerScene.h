#pragma once

#include "cocos2d.h"

class ChoosePLayer : public cocos2d::Layer
{
public:
    virtual bool init() override;
    
    static cocos2d::Scene* scene();
    
    void buttonPressedCallback(Ref* sender, int tag);
    
    // implement the "static create()" method manually
    CREATE_FUNC(ChoosePLayer);
    
private:
    
};