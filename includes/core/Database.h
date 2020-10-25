#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <sqlite3/sqlite3.h>
#include <cstdio>
#include <string>
#include <vector>

struct GenerationState
{
    uint8_t *binary;
    int size;
};

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
    static GenerationState getGenerationState(int);
};

#endif // _DATABASE_H_
