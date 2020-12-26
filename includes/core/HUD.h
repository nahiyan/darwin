#ifndef __CORE_HUD_H__
#define __CORE_HUD_H__

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class HUD
{
public:
    static Node *create(int generationCount)
    {
        auto generationCountLabel = Label::createWithTTF("Generation: " + to_string(generationCount), "fonts/arial.ttf", 16);
        generationCountLabel->setTextColor(Color4B::BLACK);
        generationCountLabel->enableOutline(Color4B::WHITE, 1);
        generationCountLabel->setPosition(generationCountLabel->getContentSize().width / 2 + 10, Director::getInstance()->getVisibleSize().height - generationCountLabel->getContentSize().height);

        return generationCountLabel;
    }
};

#endif // __CORE_HUD_H__