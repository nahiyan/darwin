#ifndef __JUMPER_MAIN_SCENE_H__
#define __JUMPER_MAIN_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include <core/EvolutionSession.h>
#include "JumperGroup.h"
#include "Jumper.h"
#include "Obstacle.h"

namespace Jumper
{
    class MainScene : public cocos2d::Scene
    {
    private:
        cocos2d::Size visibleSize;
        EvolutionSession<JumperGroup> *evolutionSession;

        struct CurrentGenerationInfo
        {
            int obstacles;
        } currentGenerationInfo;

        bool onContactBegin(cocos2d::PhysicsContact &);
        void addObstacle();
        void nextGeneration();

    public:
        static cocos2d::Scene *createScene();
        ~MainScene();

        virtual bool init();
        virtual void update(float);
        virtual void addObstacles();

        // a selector callback
        void menuCloseCallback(cocos2d::Ref *pSender);

        // implement the "static create()" method manually
        CREATE_FUNC(MainScene);
    };
} // namespace Jumper

#endif // __JUMPER_MAIN_SCENE_H__
