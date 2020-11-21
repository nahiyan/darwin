#include <extensions/wheels/Car.h>
#include <algorithm>
#include <cmath>
#include <core/Debug.h>
#include <helpers/random.h>
#include <helpers/time.h>

#define SIDE_SENSOR_WIDTH 130.0f
#define SIDE_SENSOR_HEIGHT 145.0f
#define CENTER_SENSOR_HEIGHT 190.0f
#define SCALE 0.5
#define STEERING_POWER 0.1
#define MAX_STEERING_ANGLE 2

using namespace Wheels;

USING_NS_CC;

Car::Car(std::vector<double> parameters)
{
    this->distance = 0;
    this->steeringAngle = 0;
    this->sensors[0] = 0;
    this->sensors[1] = 0;
    this->sensors[2] = 0;
    this->sensorInfo.car = this;
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

    // Neural network
    this->neuralNetwork = std::make_shared<OpenNN::NeuralNetwork>(OpenNN::NeuralNetwork::Approximation, OpenNN::Vector<size_t>{3, 10, 2});

    if (parameters.size() > 0)
    {
        this->neuralNetwork->set_parameters(parameters);
    }
    else
    {
        // Randomize parameters
        std::vector<double> parameters;
        for (int i = 0; i < this->neuralNetwork->get_parameters_number(); i++)
            parameters.push_back(Darwin::RandomHelper::nnParameter(TimeHelper::now() * i));
        this->neuralNetwork->set_parameters(parameters);
    }
}

Car::~Car()
{
}

void Car::update(float delta)
{
    // Reset sensors
    this->sensors[0] = 0;
    this->sensors[1] = 0;
    this->sensors[2] = 0;

    // Accumulate distance
    this->distance += this->node->getPhysicsBody()->getVelocity().length() * delta;

    // Raycast
    auto rayCastCB = [&](PhysicsWorld &world, const PhysicsRayCastInfo &info, void *sensorInfo) -> bool {
        if (info.shape->getBody()->getCategoryBitmask() == 2)
        {
            ((SensorInfo *)sensorInfo)->car->setSensor(((SensorInfo *)sensorInfo)->direction, 1 - info.fraction);

            return false;
        }

        return true;
    };

    auto position = this->node->getParent()->convertToWorldSpace(this->node->getPosition());
    auto angle = -this->node->getPhysicsBody()->getRotation() * (M_PI / 180);

    auto physicsWorld = ((Scene *)this->node->getParent())->getPhysicsWorld();
    auto rayStartingPosition = (position + Vec2(0, 103.5 * SCALE + 2));
    // Left
    auto rayEndingPosition1 = rayStartingPosition + Vec2(-SIDE_SENSOR_WIDTH * SCALE, SIDE_SENSOR_HEIGHT * SCALE);
    // Right
    auto rayEndingPosition2 = rayStartingPosition + Vec2(SIDE_SENSOR_WIDTH * SCALE, SIDE_SENSOR_HEIGHT * SCALE);
    // Forward
    auto rayEndingPosition3 = rayStartingPosition + Vec2(0, CENTER_SENSOR_HEIGHT * SCALE);

    this->sensorInfo.direction = Left;
    physicsWorld->rayCast(rayCastCB, rayStartingPosition.rotateByAngle(position, angle), rayEndingPosition1.rotateByAngle(position, angle), &this->sensorInfo);
    this->sensorInfo.direction = Right;
    physicsWorld->rayCast(rayCastCB, rayStartingPosition.rotateByAngle(position, angle), rayEndingPosition2.rotateByAngle(position, angle), &this->sensorInfo);
    this->sensorInfo.direction = Forward;
    physicsWorld->rayCast(rayCastCB, rayStartingPosition.rotateByAngle(position, angle), rayEndingPosition3.rotateByAngle(position, angle), &this->sensorInfo);

    // Neural network
    OpenNN::Tensor<double> input{1, 3};
    input[0] = (double)this->sensors[0];
    input[1] = (double)this->sensors[1];
    input[2] = (double)this->sensors[2];

    auto outputs = this->neuralNetwork->calculate_outputs(input);
    this->direction2D.y = outputs[0] > 0.5 ? Forward : None;
    this->direction2D.x = outputs[1] > 1 ? Right : (outputs[1] < -1 ? Left : None);

    // Movement
    if (this->direction2D.y == Forward)
        this->node->getPhysicsBody()->applyForce(Vec2(0, 2500000.0f * delta));

    if (this->direction2D.x == Left && this->steeringAngle <= MAX_STEERING_ANGLE)
        this->steeringAngle += STEERING_POWER;
    else if (this->direction2D.x == Right && this->steeringAngle >= -MAX_STEERING_ANGLE)
        this->steeringAngle -= STEERING_POWER;
    else
        this->steeringAngle -= this->steeringAngle > 0 ? STEERING_POWER : -STEERING_POWER;

    this->node->getPhysicsBody()->applyTorque(this->steeringAngle * 150000.0f * delta * this->node->getPhysicsBody()->getVelocity().length());
}

float Car::getScore()
{
    return 0;
}

void Car::setScore()
{
}

void Car::setSensor(Direction1D direction, float value)
{
    int index;
    switch (direction)
    {
    case Left:
        index = 0;
        break;
    case Forward:
        index = 1;
        break;
    default:
        index = 2;
        break;
    }
    this->sensors[index] = value;
}