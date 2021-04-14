#include "Config.h"

using namespace Core;
using namespace std;

Document Config::document;

void Config::parse(string json)
{
    document.Parse(json.c_str());
}

bool Config::keyExists(string key)
{
    return document.HasMember(key.c_str());
}

string Config::getString(string key, string default_)
{
    return document.HasMember(key.c_str()) && document[key.c_str()].IsString() ? document[key.c_str()].GetString() : default_;
}

int Config::getInt(string key, int default_)
{
    return document.HasMember(key.c_str()) ? document[key.c_str()].GetInt() : default_;
}

double Config::getDouble(string key, double default_)
{
    return document.HasMember(key.c_str()) ? document[key.c_str()].GetDouble() : default_;
}

float Config::getFloat(string key, float default_)
{
    return document.HasMember(key.c_str()) ? document[key.c_str()].GetFloat() : default_;
}
