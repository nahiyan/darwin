#include "Jumper.h"
#include <iostream>
#include <memory>
#include <opennn.h>
#include <ctime>
#include "MainScene.h"

USING_NS_CC;

Jumper::Jumper(const Vec2 &position)
{
    // Create node
    this->node = Sprite::create("jumper1.png");
    this->node->setPosition(position);

    auto physicsBody = PhysicsBody::createBox(Size(65, 70), PHYSICSBODY_MATERIAL_DEFAULT);
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(true);
    physicsBody->setCategoryBitmask(1);
    physicsBody->setCollisionBitmask(4);
    physicsBody->setContactTestBitmask(2);

    this->node->addComponent(physicsBody);

    // Reset attributes
    this->reset();

    // Initialize neural network
    this->neuralNetwork = std::make_shared<OpenNN::NeuralNetwork>(OpenNN::NeuralNetwork::Classification, OpenNN::Vector<size_t>{1, 100, 1});
    this->neuralNetwork->randomize_parameters_normal();
}

void Jumper::reset()
{
    this->deathTimestamp = 0;
    this->rayTraceFraction = 0;
    this->node->getPhysicsBody()->setVelocity(Vec2::ZERO);
}

Jumper::~Jumper()
{
}

void Jumper::setRayTraceFraction(float &fraction)
{
    this->rayTraceFraction = fraction;
}

void Jumper::update(float delta)
{
}