#ifndef __CORE_HUD_H__
#define __CORE_HUD_H__

#include "CoreSession.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

class HUD
{
private:
    static std::string hudString(float &mutationRate)
    {
        return "Generation: " + to_string(CoreSession::generationIndex) + "\nMutation Rate" + (CoreSession::hudSelection == CoreSession::MutationRate ? " (+/-)" : "") + ": " + to_string_with_precision(mutationRate * 100, 2) + "%\nSpeed " + (CoreSession::hudSelection == CoreSession::Speed ? " (+/-)" : "") + ": " + to_string_with_precision(CoreSession::speed, 2);
    }

public:
    static Label *create(float mutationRate)
    {
        auto hud = Label::createWithTTF(hudString(mutationRate), "fonts/arial.ttf", 16);
        hud->setTextColor(Color4B::BLACK);
        hud->enableOutline(Color4B::WHITE, 1);
        hud->setPosition(hud->getContentSize().width / 2 + 5, Director::getInstance()->getVisibleSize().height - hud->getContentSize().height / 2);

        return hud;
    }

    static void update(Label *node, float mutationRate)
    {
        node->setString(hudString(mutationRate));
        node->setPosition(node->getContentSize().width / 2 + 5, Director::getInstance()->getVisibleSize().height - node->getContentSize().height / 2);
    }
};

#endif // __CORE_HUD_H__
