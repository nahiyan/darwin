#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <sqlite3/sqlite3.h>

class Database
{
private:
    sqlite3 *handle;

public:
    Database(const char *);
    void close();
};

#endif // _DATABASE_H_
