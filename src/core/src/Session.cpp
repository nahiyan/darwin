#include "Session.h"

using namespace Core;

int Session::generationIndex = 0;
float Session::speed = 1;
float Session::mutationRate = .01;
int Session::populationSize = 50;
float Session::eliteFraction = .05;
float Session::fertileFraction = .3;
float Session::randomFraction = .15;
int Session::savedModelsCount = 10;
Session::HUDSelection Session::hudSelection = MutationRate;
