#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include <core/EvolutionSession.h>
#include "Jumper.h"
#include "Obstacle.h"

namespace Jumper
{
    class MainScene : public cocos2d::Scene
    {
    private:
        cocos2d::Size visibleSize;
        EvolutionSession<Jumper> *evolutionSession;

        struct CurrentGenerationInfo
        {
            std::vector<Jumper *> population;
            int obstaclesUsed;
            int obstaclesDeployed;
            int jumpersAlive;
            int totalJumpers;
            long long startTimestamp;
            int totalJumps;
        } cGInfo;

        bool onContactBegin(cocos2d::PhysicsContact &);
        void addObstacle();
        void nextGeneration();
        void onMouseMove(cocos2d::EventMouse *);

    public:
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
} // namespace Jumper

#endif // __MAIN_SCENE_H__
