#include "Flapper.h"
#include <algorithm>
#include <cmath>
#include "core/Debug.h"
#include "helpers/random.h"
#include "helpers/time.h"
#include "MainScene.h"
#include "Session.h"
#include "Constants.h"
#include "core/EvolutionCommon.h"

using namespace Flappers;

USING_NS_CC;

Flapper::Flapper(std::vector<double> parameters)
{
    this->reset();

    // Neural network
    this->neuralNetwork = std::make_shared<OpenNN::NeuralNetwork>(OpenNN::NeuralNetwork::Classification, OpenNN::Vector<size_t>{4, 20, 1});

    if (parameters.size() > 0)
        this->neuralNetwork->set_parameters(parameters);
    else
        // Randomize parameters
        Core::EvolutionCommon<Flapper>::randomize(this);
}

Flapper::~Flapper()
{
}

void Flapper::update(float delta)
{
    flapTemperature = max<double>((double)0, flapTemperature - delta);

    // Record time survived
    this->score += delta;

    // Neural network inputs
    double horizontalDistanceToPipe = 0, topPipeVerticalDistance = 0, bottomPipeVerticalDistance = 0;
    for (auto &pipe : Session::pipes)
    {
        auto pipePosition = pipe->getParent()->convertToWorldSpace(pipe->getPosition());
        auto flapperPosition = this->node->getParent()->convertToWorldSpace(this->node->getPosition());

        if (flapperPosition.x - pipePosition.x <= (FLAPPER_WIDTH / 2 + PIPE_BODY_WIDTH / 2))
        {
            horizontalDistanceToPipe = pipePosition.x - flapperPosition.x;

            auto frontPipeChildren = pipe->getChildren();

            topPipeVerticalDistance = frontPipeChildren.at(0)->getBoundingBox().getMinY() - PIPE_HEAD_HEIGHT - flapperPosition.y;
            bottomPipeVerticalDistance = flapperPosition.y - frontPipeChildren.at(2)->getBoundingBox().getMaxY() + PIPE_HEAD_HEIGHT;
            break;
        }
    }

    // Neural network
    OpenNN::Tensor<double> input{1, 4};
    input[0] = horizontalDistanceToPipe;
    input[1] = topPipeVerticalDistance;
    input[2] = bottomPipeVerticalDistance;
    input[3] = (double)this->node->getPhysicsBody()->getVelocity().y;

    auto outputs = this->neuralNetwork->calculate_outputs(input);

    // printf("Step: %.2f %.2f %.2f %.2f %.2f\n", input[0], input[1], input[2], input[3], outputs[0]);

    if (outputs[0] >= .5 && flapTemperature == 0)
        this->flap();
}

float Flapper::getScore()
{
    return this->score;
}

void Flapper::setScore()
{
    // Nothing will happen
}

void Flapper::kill()
{
    if (this->dead == false)
    {
        this->dead = true;
        MainScene::getInstance()->removeChild(this->node, true);
    }
}

bool Flapper::isDead()
{
    return this->dead;
}

void Flapper::reset()
{
    this->generateNode();
    this->score = 0;
    this->dead = false;
    this->flapTemperature = 0;
}

void Flapper::generateNode()
{
    // Sprite
    this->node = Sprite::createWithSpriteFrameName("redbird-midflap.png");
    this->node->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 400, 500));

    // Physics body
    auto physicsBody = PhysicsBody::createBox(Size(FLAPPER_WIDTH, FLAPPER_HEIGHT), PhysicsMaterial(0.1f, 0.0f, 0.1f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(true);
    physicsBody->setCategoryBitmask(1);
    physicsBody->setCollisionBitmask(6);   // Pipes + Base/Roof
    physicsBody->setContactTestBitmask(6); // Pipes + Base/Roof
    // physicsBody->setVelocityLimit(300);
    this->node->addComponent(physicsBody);

    Vector<SpriteFrame *> frames;
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    frames.pushBack(spriteFrameCache->getSpriteFrameByName("redbird-midflap.png"));
    frames.pushBack(spriteFrameCache->getSpriteFrameByName("redbird-downflap.png"));
    frames.pushBack(spriteFrameCache->getSpriteFrameByName("redbird-upflap.png"));

    auto animation = Animation::createWithSpriteFrames(frames, 1.0f / 3);
    this->node->runAction(RepeatForever::create(Animate::create(animation)));
}

void Flapper::flap()
{
    this->node->getPhysicsBody()->applyImpulse(Vec2(0, 30 * 1000));
    flapTemperature += 0.3;
}
