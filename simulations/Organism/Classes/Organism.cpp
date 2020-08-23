#include "Organism.h"
#include <iostream>

USING_NS_CC;

Organism::Organism(const Vec2 &position)
{
    // Create node
    this->node = DrawNode::create();
    this->node->setPosition(position);
    this->setGeometry();

    auto physicsBody = PhysicsBody::createCircle(20, PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(1);
    physicsBody->setCollisionBitmask(6);
    physicsBody->setContactTestBitmask(2);

    this->node->addComponent(physicsBody);

    // Initialize food intersection
    this->foodIntersection = false;

    // Initialize neural network
    this->neuralNetwork = new OpenNN::NeuralNetwork(OpenNN::NeuralNetwork::Classification, OpenNN::Vector<size_t>{2, 10, 2});
    this->neuralNetwork->randomize_parameters_normal();
}

void Organism::setGeometry()
{
    this->node->drawDot(Vec2::ZERO, 20, Color4F::GREEN);
    this->node->drawLine(Vec2::ZERO + Vec2(0, 20), Vec2::ZERO + Vec2(40, 100), this->foodIntersection ? Color4F::GREEN : Color4F::RED);
    this->node->drawLine(Vec2::ZERO + Vec2(0, 20), Vec2::ZERO + Vec2(-40, 100), this->foodIntersection ? Color4F::GREEN : Color4F::RED);
}

void Organism::resetGeometry()
{
    this->node->clear();
    this->setGeometry();
}

Organism::~Organism()
{
    delete this->neuralNetwork;
}
