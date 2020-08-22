#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include "Organism.h"

class MainScene : public cocos2d::Scene
{
private:
    std::vector<Organism> organismList;
    bool onContactBegin(cocos2d::PhysicsContact &);

    void addFood();
    int index;

public:
    static cocos2d::Scene *createScene();

    virtual bool init();
    virtual void update(float);
    virtual void organismsTick(float);
    void onMouseMove(cocos2d::EventMouse *);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref *pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif // __HELLOWORLD_SCENE_H__
