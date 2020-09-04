#include "Organism.h"
#include <iostream>
#include <memory>
#include <neural_network.h>
#include <ctime>

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

    // Food eaten count
    this->foodEaten = 0;

    // Label
    this->label = Label::createWithTTF(std::to_string(this->foodEaten), "fonts/arial.ttf", 12);
    this->label->setTextColor(Color4B::BLACK);
    this->node->addChild(this->label);

    // Initialize food intersection
    this->foodIntersection = false;

    // Targeted food
    this->targetedFoodName = "";

    // Last velocity change timestamp
    this->lastVelocityChangeTimestamp = 0;

    // Initialize neural network
    this->neuralNetwork = std::make_shared<OpenNN::NeuralNetwork>(OpenNN::NeuralNetwork::Classification, OpenNN::Vector<size_t>{2, 20, 2});
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
    OpenNN::Tensor<double> input{1, 2};
    input[0] = (double)this->getFoodIntersection();
    input[1] = this->node->getPhysicsBody()->getVelocity().length();

    auto output = this->neuralNetwork->calculate_outputs(input);

    bool goAfterFood = output[0] > 0.5 ? true : false;
    bool shouldChangeVelocity = output[1] > 0.5 ? true : false;

    bool canChangeVelocity = (std::time(nullptr) - this->lastVelocityChangeTimestamp) >= 1;

    bool willChangeVelocity = canChangeVelocity && shouldChangeVelocity;

    if (goAfterFood && willChangeVelocity && this->targetedFoodName.length() > 0)
    {
        auto targetedFood = this->node->getParent()->getChildByName(this->targetedFoodName);

        if (targetedFood != nullptr)
        {

            int speedX = random(60, 130);
            int speedY = random(60, 130);
            auto foodPosition = targetedFood->getParent()->convertToWorldSpace(targetedFood->getPosition());

            Vec2 newVelocity(random(60, 130), random(60, 130));
            newVelocity.rotateByAngle(Vec2(0, 0), Vec2(0, 1).getAngle(foodPosition));

            this->node->getPhysicsBody()->setVelocity(foodPosition);
            this->lastVelocityChangeTimestamp = std::time(nullptr);
        }
    }
    else if (!goAfterFood && willChangeVelocity)
    {
        int speedX = random(60, 130);
        int speedY = random(60, 130);

        int directionX = random(-1, 1) > 0 ? 1 : -1;
        int directionY = random(-1, 1) > 0 ? 1 : -1;

        this->node->getPhysicsBody()->setVelocity(Vec2(speedX * directionX, speedY * directionY));
        this->lastVelocityChangeTimestamp = std::time(nullptr);
    }
}