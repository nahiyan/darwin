#ifndef __CORE_HUD_H__
#define __CORE_HUD_H__

#include "CoreSession.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class HUD
{
public:
    static Label *create()
    {
        auto generationIndexLabel = Label::createWithTTF("Generation: " + to_string(CoreSession::generationIndex), "fonts/arial.ttf", 16);
        generationIndexLabel->setTextColor(Color4B::BLACK);
        generationIndexLabel->enableOutline(Color4B::WHITE, 1);
        generationIndexLabel->setPosition(generationIndexLabel->getContentSize().width / 2 + 10, Director::getInstance()->getVisibleSize().height - generationIndexLabel->getContentSize().height);

        return generationIndexLabel;
    }

    static void update(Label *node)
    {
        node->setString("Generation: " + to_string(CoreSession::generationIndex));
    }
};

#endif // __CORE_HUD_H__
