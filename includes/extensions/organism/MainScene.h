#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include <ctime>
#include <map>
#include "Organism.h"
#include "Evolution.h"

class MainScene : public cocos2d::Scene
{
private:
    cocos2d::Size visibleSize;
    int foodToBeAdded;
    int totalFoodEaten;
    std::time_t generationStartTimestamp;
    Evolution *evolutionSession;
    int index;

    bool onContactBegin(cocos2d::PhysicsContact &);
    void addFood(int);
    void prepareNextGeneration();

public:
    std::map<int, Organism *> organismMap;

    static cocos2d::Scene *createScene();
    ~MainScene();

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
