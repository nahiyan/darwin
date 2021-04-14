#ifndef __CORE_CONFIG_H__
#define __CORE_CONFIG_H__

#include "rapidjson/document.h"
#include <string>

using namespace rapidjson;
using namespace std;

namespace Core
{
    class Config
    {
        static Document document;

    public:
        static void parse(string);
        static bool keyExists(string);
        static string getString(string, string);
        static int getInt(string, int);
        static double getDouble(string, double);
        static float getFloat(string, float);
    };
}
#endif // __CORE_CONFIG_H__
