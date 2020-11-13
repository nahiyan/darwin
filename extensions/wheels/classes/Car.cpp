#include <extensions/wheels/Car.h>
#include <algorithm>
#include <cmath>
#include <core/Debug.h>

#define SIDE_SENSOR_WIDTH 130.0f
#define SIDE_SENSOR_HEIGHT 145.0f
#define CENTER_SENSOR_HEIGHT 175.0f
#define SCALE 0.5

using namespace Wheels;

USING_NS_CC;

Car::Car()
{
    this->direction2D = {None, None};
    auto sprite = Sprite::create("wheels/Ambulance.png");
    sprite->setScale(SCALE);
    sprite->setPosition(Vec2(100, 50));
    this->node = sprite;

    // Physics body
    auto physicsBody = PhysicsBody::createBox(Size(102, 207), PhysicsMaterial(0.1f, 0.0f, 0.1f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(1);
    physicsBody->setCollisionBitmask(2);   // Tracks
    physicsBody->setContactTestBitmask(2); // Tracks
    physicsBody->setLinearDamping(0.8f);
    physicsBody->setAngularDamping(0.8f);
    sprite->addComponent(physicsBody);

    auto sensors = DrawNode::create();
    sensors->drawLine(Vec2(51, 207), Vec2(51 + SIDE_SENSOR_WIDTH, 207 + SIDE_SENSOR_HEIGHT), Color4F::GREEN);
    sensors->drawLine(Vec2(51, 207), Vec2(51 - SIDE_SENSOR_WIDTH, 207 + SIDE_SENSOR_HEIGHT), Color4F::GREEN);
    sensors->drawLine(Vec2(51, 207), Vec2(51, 207 + CENTER_SENSOR_HEIGHT), Color4F::GREEN);
    sprite->addChild(sensors);
}

Car::~Car()
{
}

void Car::update(float delta)
{
    // Raycast
    auto rayCastCB = [&, delta](PhysicsWorld &world, const PhysicsRayCastInfo &info, void *organism) -> bool {
        if (info.shape->getBody()->getCategoryBitmask() == 2)
        {
            // ((Organism *)organism)->setFoodIntersection();

            return false;
        }

        return true;
    };

    auto position = this->node->getParent()->convertToWorldSpace(this->node->getPosition());
    auto angle = -this->node->getPhysicsBody()->getRotation() * (M_PI / 180);

    auto physicsWorld = ((Scene *)this->node->getParent())->getPhysicsWorld();
    auto rayStartingPosition = (position + Vec2(0, 103.5 * SCALE + 2));
    auto rayEndingPosition1 = rayStartingPosition + Vec2(-SIDE_SENSOR_WIDTH * SCALE, SIDE_SENSOR_HEIGHT * SCALE);
    auto rayEndingPosition2 = rayStartingPosition + Vec2(SIDE_SENSOR_WIDTH * SCALE, SIDE_SENSOR_HEIGHT * SCALE);
    auto rayEndingPosition3 = rayStartingPosition + Vec2(0, CENTER_SENSOR_HEIGHT * SCALE);

    physicsWorld->rayCast(rayCastCB, rayStartingPosition.rotateByAngle(position, angle), rayEndingPosition1.rotateByAngle(position, angle), this);
    physicsWorld->rayCast(rayCastCB, rayStartingPosition.rotateByAngle(position, angle), rayEndingPosition2.rotateByAngle(position, angle), this);
    physicsWorld->rayCast(rayCastCB, rayStartingPosition.rotateByAngle(position, angle), rayEndingPosition3.rotateByAngle(position, angle), this);

    if (this->direction2D.y == Forward)
        this->node->getPhysicsBody()->applyForce(Vec2(0, 5000000.0f * delta));

    int torqueDirection;
    if (this->direction2D.x == Left)
        torqueDirection = 1;
    else if (this->direction2D.x == Right)
        torqueDirection = -1;
    else
        torqueDirection = 0;

    this->node->getPhysicsBody()->applyTorque(torqueDirection * 200000.0f * delta * this->node->getPhysicsBody()->getVelocity().length());
}
