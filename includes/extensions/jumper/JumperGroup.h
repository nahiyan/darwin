#ifndef __JUMPERGROUP_H__
#define __JUMPERGROUP_H__

#include <opennn/neural_network.h>
#include "Jumper.h"
#include "IGroup.h"
#include "cocos2d.h"

namespace Jumper
{
    class JumperGroup : public IGroup
    {
    private:
        float score;

    public:
        Jumper *members[3];

        JumperGroup(std::vector<double>);
        ~JumperGroup();

        float getScore();
        void setScore();
        void reset();
        void update(float);
        void kill(cocos2d::Node *);
        void positionNodes();
    };
} // namespace Jumper

#endif // __JUMPERGROUP_H__
