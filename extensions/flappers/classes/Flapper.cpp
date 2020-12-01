#include <extensions/flappers/Flapper.h>
#include <algorithm>
#include <cmath>
#include <core/Debug.h>
#include <helpers/random.h>
#include <helpers/time.h>
#include <extensions/flappers/MainScene.h>
#include <extensions/flappers/Session.h>

using namespace Flappers;

USING_NS_CC;

Flapper::Flapper(std::vector<double> parameters)
{
    this->reset();

    // Neural network
    // this->neuralNetwork = std::make_shared<OpenNN::NeuralNetwork>(OpenNN::NeuralNetwork::Approximation, OpenNN::Vector<size_t>{3, 10, 2});

    // if (parameters.size() > 0)
    // {
    //     this->neuralNetwork->set_parameters(parameters);
    // }
    // else
    // {
    //     // Randomize parameters
    //     std::vector<double> parameters;
    //     for (int i = 0; i < this->neuralNetwork->get_parameters_number(); i++)
    //         parameters.push_back(Darwin::RandomHelper::nnParameter(TimeHelper::now() * i));
    //     this->neuralNetwork->set_parameters(parameters);
    // }
}

Flapper::~Flapper()
{
}

void Flapper::update(float delta)
{
}

float Flapper::getScore()
{
    return this->score;
}

void Flapper::setScore()
{
}

void Flapper::kill()
{
    if (this->dead == false)
    {
        this->dead = true;
        MainScene::getInstance()->removeChild(this->node);
        Session::decrementFlapperQuantity();
    }
}

bool Flapper::isDead()
{
    return this->dead;
}

void Flapper::reset()
{
    this->distance = 0;
    this->score = 0;
    this->dead = false;
    this->generateNode();
}

void Flapper::generateNode()
{
    // // Sprite
    this->node = Sprite::createWithSpriteFrameName("redbird-midflap.png");
    this->node->setPosition(Vec2(100, 500));

    // Physics body
    auto physicsBody = PhysicsBody::createBox(Size(34, 24), PhysicsMaterial(0.1f, 0.0f, 0.1f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(true);
    physicsBody->setCategoryBitmask(1);
    physicsBody->setCollisionBitmask(2);   // Pipes
    physicsBody->setContactTestBitmask(2); // Pipes
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
}