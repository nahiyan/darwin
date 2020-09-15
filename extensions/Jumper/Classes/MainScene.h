#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include <ctime>
#include "Jumper.h"
#include "Obstacle.h"
#include "Evolution.h"

class MainScene : public cocos2d::Scene
{
private:
    cocos2d::Size visibleSize;
    std::time_t generationStartTimestamp;
    Evolution *evolutionSession;
    int obstaclesUsed;

    bool onContactBegin(cocos2d::PhysicsContact &);
    void addObstacle();
    void nextGeneration();
    void onMouseMove(cocos2d::EventMouse *);

public:
    std::vector<Jumper *> jumperList;

    static cocos2d::Scene *createScene();
    ~MainScene();

    virtual bool init();
    virtual void update(float);
    virtual void addObstacle(float);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref *pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif // __MAIN_SCENE_H__
