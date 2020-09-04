#include "Evolution.h"
#include "Organism.h"
#include <cocos2d.h>

USING_NS_CC;

void Evolution::nextGeneration(vector<Organism *> organismList)
{
    std::cout << "New generation! ";
    auto visibleSize = Director::getInstance()->getVisibleSize();
    for (auto organism : organismList)
    {
        organism->node->getPhysicsBody()->setVelocity(Vec2::ZERO);
        organism->node->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    }
}