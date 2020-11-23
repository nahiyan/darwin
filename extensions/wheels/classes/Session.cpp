#include <extensions/wheels/Session.h>

using namespace Wheels;

EvolutionSession<Car> *Session::evolutionSession = nullptr;
Session::CurrentGenerationInfo Session::currentGenerationInfo = CurrentGenerationInfo{0};

void Session::decrementCarQuantity()
{
    Session::currentGenerationInfo.carQuantity--;

    if (Session::currentGenerationInfo.carQuantity == 0)
        Session::nextGeneration();
}

void Session::setCarQuantity(int quantity)
{
    Session::currentGenerationInfo.carQuantity = quantity;
}

void Session::nextGeneration()
{
    log("Next generation!");
}
