#include <extensions/flappers/Pipe.h>

#define BASE_HEIGHT 112
#define MIDDLE_SPACE 200.0f

using namespace Flappers;
USING_NS_CC;

Node *Pipe::create(float topFraction)
{
    auto screenHeight = Director::getInstance()->getVisibleSize().height;
    auto verticalSpace = screenHeight - BASE_HEIGHT - 48;
    auto topPipeHeight = topFraction * verticalSpace - (MIDDLE_SPACE / 2);
    auto bottomPipeHeight = verticalSpace - topPipeHeight - (MIDDLE_SPACE / 2);

    auto pipe = Node::create();
    pipe->setPosition(500, 0);

    auto topPipeBody = Sprite::createWithSpriteFrameName("pipe-green-body.png");
    topPipeBody->setPosition(0, screenHeight - (topPipeHeight / 2));
    topPipeBody->setScaleY(topPipeHeight / 296);
    topPipeBody->setFlippedY(true);
    pipe->addChild(topPipeBody);

    auto topPipeHead = Sprite::createWithSpriteFrameName("pipe-green-head.png");
    topPipeHead->setPosition(0, screenHeight - topPipeHeight - 12);
    pipe->addChild(topPipeHead);

    auto bottomPipeBody = Sprite::createWithSpriteFrameName("pipe-green-body.png");
    bottomPipeBody->setPosition(0, (bottomPipeHeight / 2) + BASE_HEIGHT);
    bottomPipeBody->setScaleY(bottomPipeHeight / 296);
    pipe->addChild(bottomPipeBody);

    auto bottomPipeHead = Sprite::createWithSpriteFrameName("pipe-green-head.png");
    bottomPipeHead->setPosition(0, bottomPipeHeight + BASE_HEIGHT + 12);
    pipe->addChild(bottomPipeHead);

    return pipe;
}