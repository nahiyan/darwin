#include <core/Session.h>

using namespace Core;

int Session::extensionId = 0;
int Session::sessionId = 0;
int Session::generationId = 0;
int Session::generationIndex = 1;
float Session::speed = 1;
Session::HUDSelection Session::hudSelection = MutationRate;
