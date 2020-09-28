#include <sqlite3/sqlite3.h>
#include <cstdio>
#include <core/Database.h>

void sqlite_error()
{
    printf("Something went wrong with SQLite3.\n");
}

Database::Database(const char *path)
{
    if (sqlite3_open(path, &this->handle) != SQLITE_OK)
        sqlite_error();
}

void Database::close()
{
    if (sqlite3_close(this->handle) != SQLITE_OK)
        sqlite_error();
}

// int main()
// {
//     // Open DB
//     sqlite3 *db;
//     if (sqlite3_open("test.sqlite", &db) != SQLITE_OK)
//     {
//         sqlite_error();
//         return 0;
//     }

//     // Statement
//     char *error;

//     auto cb = [](void *a, int b, char **c, char **d) -> int {
//         printf("Called!\n");
//         return 0;
//     };
//     sqlite3_exec(db, "CREATE TABLE generations (id INTEGER PRIMARY KEY, age INTEGER);", cb, NULL, &error);
//     sqlite3_free(error);

//     printf("%s\n", error);

//     // Close DB
//     if (sqlite3_close(db) != SQLITE_OK)
//     {
//         sqlite_error();
//         return 0;
//     }
// }