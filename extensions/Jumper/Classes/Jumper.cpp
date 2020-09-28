#include "Jumper.h"
#include <iostream>
#include <memory>
#include <opennn/opennn.h>
#include <ctime>
#include "MainScene.h"

USING_NS_CC;

Jumper::Jumper(const Vec2 &position, int index)
{
    this->index = index;

    this->generateNode();
    this->node->setPosition(position);

    // Reset attributes
    this->prepareForNewGeneration();

    // Initialize neural network
    this->neuralNetwork = std::make_shared<OpenNN::NeuralNetwork>(OpenNN::NeuralNetwork::Classification, OpenNN::Vector<size_t>{1, 100, 1});
    this->neuralNetwork->randomize_parameters_normal();
}

void Jumper::generateNode()
{
    // Create node
    this->node = Sprite::create("jumper1.png");

    auto physicsBody = PhysicsBody::createBox(Size(65, 70), PhysicsMaterial(1.0f, 0.0f, 1.0f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(true);
    physicsBody->setCategoryBitmask(1);
    physicsBody->setCollisionBitmask(6);   // 2 + 4
    physicsBody->setContactTestBitmask(2); // 2

    this->node->addComponent(physicsBody);
}

void Jumper::prepareForNewGeneration()
{
    this->isDead = false;
    this->deathTimestamp = 0;
    this->lastJumpTimestamp = 0;
    this->rayTraceFraction = 0;
}

Jumper::~Jumper()
{
}

void Jumper::setRayTraceFraction(float fraction)
{
    this->rayTraceFraction = fraction;
}

void Jumper::update(float delta)
{
    // Ray trace
    auto physicsWorld = ((Scene *)this->node->getParent())->getPhysicsWorld();

    this->rayTraceFraction = 0;

    auto rayCastCB = [](PhysicsWorld &world, const PhysicsRayCastInfo &info, void *jumper) -> bool {
        if (info.shape->getBody()->getCategoryBitmask() == 2)
        {
            ((Jumper *)jumper)->setRayTraceFraction(1 - info.fraction);

            return false;
        }

        return true;
    };

    auto jumperPosition = this->node->getParent()->convertToWorldSpace(this->node->getPosition());

    physicsWorld->rayCast(rayCastCB, jumperPosition + Vec2(32.6, -25), jumperPosition + Vec2(117.5, -25), this);

    // Neural network
    OpenNN::Tensor<double> input{1, 1};
    input[0] = (double)this->rayTraceFraction;
    auto shouldJump = this->neuralNetwork->calculate_outputs(input)[0] > 0.5;

    if (shouldJump && (std::time(nullptr) - this->lastJumpTimestamp) > 2)
    {
        this->lastJumpTimestamp = std::time(nullptr);
        auto jump = JumpBy::create(1.8, Vec2(0, 150), 150, 1);
        this->node->runAction(jump);
    }
}

int &Jumper::getIndex()
{
    return this->index;
}

float Jumper::getScore()
{
    if (this->isDead)
        return this->generationStartTimestamp - this->deathTimestamp;
    else
        return 0;
}

void Jumper::kill()
{
    this->deathTimestamp = std::time(nullptr);
    this->isDead = true;
}