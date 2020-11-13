#ifndef __Wheels_MAIN_SCENE_H__
#define __Wheels_MAIN_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include <core/EvolutionSession.h>
#include "Car.h"

namespace Wheels
{
    class MainScene : public cocos2d::Scene
    {
    private:
        cocos2d::Size visibleSize;

        struct CurrentGenerationInfo
        {
            // TO DO: PUT CURRENT GENERATION INFORMATION AS REQUIRED.
        } cGInfo;

        EvolutionSession<Car> *evolutionSession;

    public:
        static cocos2d::Scene *createScene();
        ~MainScene();

        virtual bool init();
        virtual void update(float);

        // a selector callback
        void menuCloseCallback(cocos2d::Ref *pSender);

        // implement the "static create()" method manually
        CREATE_FUNC(MainScene);
    };
} // namespace Wheels

#endif // __Wheels_MAIN_SCENE_H__
