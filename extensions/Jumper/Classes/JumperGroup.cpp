#include <extensions/jumper/JumperGroup.h>
#include <extensions/jumper/Jumper.h>
#include <helpers/random.h>
#include <helpers/time.h>
#include <memory>
#include "cocos2d.h"
#define JUMPER_POSITION Vec2(Director::getInstance()->getVisibleSize().width - 134, 50)

USING_NS_CC;

Jumper::JumperGroup::JumperGroup(std::vector<double> parameters)
{
    // Initialize parameters
    for (auto &member : this->members)
    {
        member = new Jumper;
        member->group = this;
    }
    this->positionNodes();

    this->score = 0;

    // Initialize neural network
    this->neuralNetwork = std::make_shared<OpenNN::NeuralNetwork>(OpenNN::NeuralNetwork::Classification, OpenNN::Vector<size_t>{1, 10, 1});

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

Jumper::JumperGroup::~JumperGroup()
{
    for (int i = 0; i < 3; i++)
        delete this->members[i];
}

void Jumper::JumperGroup::setScore()
{
    for (auto &member : this->members)
        if (!member->isDead)
            if (member->getJumps() > 0)
                this->score += 1.0f / (float)member->getJumps();
}

float Jumper::JumperGroup::getScore()
{
    return this->score;
}

void Jumper::JumperGroup::reset()
{
    this->score = 0;
    for (auto &member : this->members)
        member->reset();
}

void Jumper::JumperGroup::update(float delta)
{
    for (auto &member : this->members)
        if (!member->isDead)
            member->update(delta);
}

void Jumper::JumperGroup::kill(cocos2d::Node *node)
{
    for (auto &member : this->members)
        if (member->node == node)
            member->isDead = true;
}

void Jumper::JumperGroup::positionNodes()
{
    auto position = JUMPER_POSITION;

    for (auto &member : this->members)
    {
        member->node->setPosition(position);
        position.subtract(Vec2(224, 0));
    }
}