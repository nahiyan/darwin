#include <extensions/jumper/Jumper.h>
#include <iostream>
#include <memory>
#include <opennn/opennn.h>
#include <extensions/jumper/MainScene.h>

#define JUMP_IMPULSE 1500 * 1000

USING_NS_CC;

Jumper::Jumper::Jumper()
{
    this->reset();
}

void Jumper::Jumper::generateNode()
{
    // Create node
    this->node = Sprite::createWithSpriteFrameName("jumper1.png");

    auto physicsBody = PhysicsBody::createBox(Size(65, 70), PhysicsMaterial(1.0f, 0.0f, 1.0f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(true);
    physicsBody->setCategoryBitmask(1);
    physicsBody->setCollisionBitmask(6);   // 2 + 4
    physicsBody->setContactTestBitmask(2); // 2
    physicsBody->setRotationEnable(false);

    this->node->addComponent(physicsBody);
}

Jumper::Jumper::~Jumper()
{
}

void Jumper::Jumper::setRayTraceFraction(float fraction)
{
    this->rayTraceFraction = fraction;
}

void Jumper::Jumper::update(float delta)
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

    if (jumperPosition.y < 0)
        log("Unexpected: Jumper escaped border; y = %f ", jumperPosition.y);

    physicsWorld->rayCast(rayCastCB, jumperPosition + Vec2(32.6, -25), jumperPosition + Vec2(117.5, -25), this);

    // Neural network
    OpenNN::Tensor<double> input{1, 1};
    input[0] = (double)this->rayTraceFraction;
    auto shouldJump = this->group->neuralNetwork->calculate_outputs(input)[0] > 0.5;

    if (shouldJump && this->node->getPhysicsBody()->getPosition().y <= 50)
    {
        this->node->getPhysicsBody()->applyImpulse(Vec2(0, JUMP_IMPULSE));
        this->jumps++;
    }
}

int Jumper::Jumper::getJumps()
{
    return this->jumps;
}

void Jumper::Jumper::reset()
{
    this->isDead = false;
    this->setRayTraceFraction(0);
    this->jumps = 0;
    this->generateNode();
}