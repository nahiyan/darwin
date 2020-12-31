#ifndef _CORE_DATABASE_H_
#define _CORE_DATABASE_H_

#include <sqlite3/sqlite3.h>
#include <cstdio>
#include <string>
#include <vector>

namespace Core {
class Database
{
private:
public:
    static sqlite3 *handle;
    static void open(std::string);

    static void close();

    static void clearSession(int);
    static int getExtensionId(std::string);

    static std::vector<std::string> getExtensionNames();

    static std::vector<int> getSessionIds(std::string);

    static std::vector<int> getGenerationIds(int);

    static int addSession(int);

    static void addGeneration(int, uint8_t *, int);
    static uint8_t *getGenerationState(int);
};}

#endif // _CORE_DATABASE_H_
