#include <core/Session.h>

using namespace Core;

int Session::extensionId = 0;
int Session::sessionId = 0;
int Session::generationId = 0;
int Session::generationIndex = 1;
float Session::speed = 1;
float Session::mutationRate = .01;
int Session::populationSize = 50;
float Session::eliteFraction = .05;
float Session::fertileFraction = .3;
float Session::randomFraction = .15;
Session::HUDSelection Session::hudSelection = MutationRate;
