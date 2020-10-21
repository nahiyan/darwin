#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <sqlite3/sqlite3.h>
#include <cstdio>
#include <string>
#include <vector>

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

    static std::vector<int> getGenerationIds(std::string, int);

    static int addSession(int);

    static void addGeneration(int, uint8_t *, int);
};

#endif // _DATABASE_H_
