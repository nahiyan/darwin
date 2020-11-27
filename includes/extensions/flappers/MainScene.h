#ifndef __FLAPPERS_MAIN_SCENE_H__
#define __FLAPPERS_MAIN_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include <core/EvolutionSession.h>

namespace Flappers
{
    class MainScene : public cocos2d::Scene
    {
    private:
        static MainScene *instance;

    public:
        static cocos2d::Scene *createScene();
        ~MainScene();
        virtual bool init();
        virtual void update(float);
        static MainScene *getInstance();
        void addPipe(float);

        // a selector callback
        void menuCloseCallback(cocos2d::Ref *pSender);

        // implement the "static create()" method manually
        CREATE_FUNC(MainScene);
    };
} // namespace Flappers

#endif // __FLAPPERS_MAIN_SCENE_H__
