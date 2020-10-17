#include "Jumper.h"
#include <iostream>
#include <memory>
#include <opennn/opennn.h>
#include <helpers/random.h>
#include <helpers/time.h>
#include "MainScene.h"

#define JUMP_IMPULSE 1500 * 1000

USING_NS_CC;

Jumper::Jumper(const Vec2 &position, int index)
{
    this->index = index;

    this->generateNode();
    this->node->setPosition(position);

    // Reset attributes
    this->prepareForNewGeneration();

    // Initialize neural network
    this->neuralNetwork = std::make_shared<OpenNN::NeuralNetwork>(OpenNN::NeuralNetwork::Classification, OpenNN::Vector<size_t>{1, 10, 1});

    // Randomize parameters
    std::vector<double> parameters;
    for (int i = 0; i < this->neuralNetwork->get_parameters_number(); i++)
        parameters.push_back(Darwin::RandomHelper::nnParameter(TimeHelper::now() * i));
    this->neuralNetwork->set_parameters(parameters);
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
    physicsBody->setRotationEnable(false);

    this->node->addComponent(physicsBody);
}

void Jumper::prepareForNewGeneration()
{
    this->isDead = false;
    this->deathTimestamp = 0;
    this->rayTraceFraction = 0;
    this->score = 0;
    this->jumps = 0;
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

    // if (jumperPosition.y < 35)
    //     this->node->setPositionY(35);

    if (jumperPosition.y < 0)
        log("%f ", jumperPosition.y);

    physicsWorld->rayCast(rayCastCB, jumperPosition + Vec2(32.6, -25), jumperPosition + Vec2(117.5, -25), this);

    // Neural network
    OpenNN::Tensor<double> input{1, 1};
    input[0] = (double)this->rayTraceFraction;
    auto shouldJump = this->neuralNetwork->calculate_outputs(input)[0] > 0.5;

    if (shouldJump && this->node->getPhysicsBody()->getPosition().y <= 40)
    {
        this->node->getPhysicsBody()->applyImpulse(Vec2(0, JUMP_IMPULSE));
        this->jumps++;
    }
}

int &Jumper::getIndex()
{
    return this->index;
}

void Jumper::setScore(long long generationStartTimestamp, long long generationDuration, int obstaclesDeployed)
{
    if (this->isDead)
    {
        float survivalDuration = this->deathTimestamp - generationStartTimestamp;
        float obstaclesFaced = ((float)obstaclesDeployed) * (survivalDuration / ((float)generationDuration));
        float jumpRatio = ((float)this->jumps) / obstaclesFaced;
        float penalty = jumpRatio * 2.0f;
        this->score = (survivalDuration / 1000.0f) - penalty;
    }
    else
    {
        this->score = 0;
    }
}

float Jumper::getScore()
{
    return this->score;
}

void Jumper::kill()
{
    this->deathTimestamp = TimeHelper::now();
    this->isDead = true;
}
