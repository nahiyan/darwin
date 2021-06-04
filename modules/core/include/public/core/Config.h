#ifndef __CORE_CONFIG_H__
#define __CORE_CONFIG_H__

#include <string>
#ifdef __CORE_CONFIG_CPP__
#include "rapidjson/document.h"
using namespace rapidjson;
#endif

using namespace std;

namespace Core
{
    class Config
    {
#ifdef __CORE_CONFIG_CPP__
        static Document document;
#endif

    public:
        static void parse(string);
        static bool keyExists(string);
        static string getString(string, string);
        static int getInt(string, int);
        static double getDouble(string, double);
        static bool getBool(string, bool);
        static float getFloat(string, float);
    };
}
#endif // __CORE_CONFIG_H__
