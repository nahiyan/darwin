#include "Organism.h"
#include <iostream>
#include <memory>
#include <opennn.h>
#include <ctime>
#include "MainScene.h"

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
    physicsBody->setCollisionBitmask(4);   // 4
    physicsBody->setContactTestBitmask(2); // 2

    this->node->addComponent(physicsBody);

    // Food eaten count
    this->foodEaten = 0;

    // Label
    this->label = Label::createWithTTF(std::to_string(this->foodEaten), "fonts/arial.ttf", 12);
    this->label->setTextColor(Color4B::BLACK);
    this->node->addChild(this->label);

    // Initialize food intersection
    this->foodIntersection = false;

    // Timestamps
    this->lastVelocityChangeTimestamp = 0;
    this->lastSuckInTimestamp = 0;

    this->suckingIn = false;

    // Initialize neural network
    this->neuralNetwork = std::make_shared<OpenNN::NeuralNetwork>(OpenNN::NeuralNetwork::Classification, OpenNN::Vector<size_t>{1, 100, 1});
    this->neuralNetwork->randomize_parameters_normal();
}

void Organism::reset()
{
    this->foodEaten = 0;
    this->lastVelocityChangeTimestamp = 0;
    this->lastSuckInTimestamp = 0;
    this->label->setString("0");
    this->node->getPhysicsBody()->setVelocity(Vec2::ZERO);
}

void Organism::setGeometry()
{
    this->node->drawDot(Vec2::ZERO, 20, this->suckingIn ? Color4F::MAGENTA : Color4F::GREEN);
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
}

void Organism::setFoodIntersection()
{
    this->foodIntersection = true;
    this->resetGeometry();
}

void Organism::unsetFoodIntersection()
{
    this->foodIntersection = false;
    this->resetGeometry();
}

bool Organism::getFoodIntersection()
{
    return this->foodIntersection;
}

void Organism::eatFood()
{
    this->label->setString(std::to_string(++this->foodEaten));
}

void Organism::update(float delta)
{
    auto rayCastCB = [](PhysicsWorld &world, const PhysicsRayCastInfo &info, void *organism) -> bool {
        if (info.shape->getBody()->getCategoryBitmask() == 2)
        {
            ((Organism *)organism)->setFoodIntersection();

            return false;
        }

        return true;
    };

    // Raycast to determine if food is intersecting the antennae
    auto organismPosition = this->node->getParent()->convertToWorldSpace(this->node->getPosition());

    this->unsetFoodIntersection();
    auto organismAngle = -this->node->getPhysicsBody()->getRotation() * (M_PI / 180);

    auto rayCastStartingPosition = (organismPosition + Vec2(0, 20)).rotateByAngle(organismPosition, organismAngle);
    auto rayCastEndingPosition1 = (organismPosition + Vec2(40, 100)).rotateByAngle(organismPosition, organismAngle);
    auto rayCastEndingPosition2 = (organismPosition + Vec2(-40, 100)).rotateByAngle(organismPosition, organismAngle);

    auto physicsWorld = ((Scene *)this->node->getParent())->getPhysicsWorld();

    physicsWorld->rayCast(rayCastCB, rayCastStartingPosition, rayCastEndingPosition1, this);
    physicsWorld->rayCast(rayCastCB, rayCastStartingPosition, rayCastEndingPosition2, this);

    // Set organism rotation
    auto currentVelocity = this->node->getPhysicsBody()->getVelocity();
    float desiredRotation = currentVelocity.getAngle(Vec2(0, 1)) * (180 / M_PI);
    float currentRotation = this->node->getPhysicsBody()->getRotation();
    float absRotDiff = abs(desiredRotation - currentRotation);
    float rotationChange = std::min(absRotDiff, 1.5f);

    if (desiredRotation > currentRotation)
        this->node->setRotation(currentRotation + rotationChange);
    else if (desiredRotation < currentRotation)
        this->node->setRotation(currentRotation - rotationChange);

    // Neural network
    bool suckingInBefore = this->suckingIn;
    this->suckingIn = (std::time(nullptr) - this->lastSuckInTimestamp) <= 1;

    if (this->suckingIn != suckingInBefore)
    {
        this->resetGeometry();
        if (this->suckingIn)
            this->node->getPhysicsBody()->setCollisionBitmask(6);
        else
            this->node->getPhysicsBody()->setCollisionBitmask(4);
    }

    if (!this->suckingIn)
    {
        OpenNN::Tensor<double> input{1, 1};
        input[0] = (double)this->getFoodIntersection();

        auto shouldSuckIn = this->neuralNetwork->calculate_outputs(input)[0] > 0.5;

        bool canChangeVelocity = (std::time(nullptr) - this->lastVelocityChangeTimestamp) >= 2;
        bool canSuckIn = (std::time(nullptr) - this->lastSuckInTimestamp) >= 5;

        if (shouldSuckIn && canSuckIn)
        {
            // this->node->getPhysicsBody()->setVelocity(Vec2::ZERO);
            this->lastSuckInTimestamp = std::time(nullptr);
        }
        else if (canChangeVelocity)
        {
            int speedX = random(60, 130);
            int speedY = random(60, 130);

            int directionX = random<int>(0, 1) == 1 ? 1 : -1;
            int directionY = random<int>(0, 1) == 1 ? 1 : -1;

            this->node->getPhysicsBody()->setVelocity(Vec2(speedX * directionX, speedY * directionY));
            this->lastVelocityChangeTimestamp = std::time(nullptr);
        }
    }
    else
    {
        auto &organismMap = ((MainScene *)this->node->getParent())->organismMap;

        auto children = this->node->getParent()->getChildren();
        for (auto child : children)
        {
            auto physicsBody = child->getPhysicsBody();
            auto organismPosition = this->node->convertToWorldSpace(this->node->getPosition());
            auto childPosition = child->convertToWorldSpace(child->getPosition());

            if (physicsBody != nullptr && physicsBody->getCategoryBitmask() == 2 && organismPosition.distance(childPosition) <= 300)
            {
                auto force = organismPosition - childPosition;
                force.normalize();
                force.scale(2000);
                physicsBody->applyForce(force);
            }
        }
    }
}

int Organism::getFoodEaten()
{
    return this->foodEaten;
}

bool Organism::isSuckingIn()
{
    return this->suckingIn;
}