#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <sqlite3/sqlite3.h>
#include <cstdio>
#include <string>
#include <vector>

void sqlite_error()
{
    printf("Something went wrong with SQLite3.\n");
}

class Database
{
private:
    static sqlite3 *handle;

public:
    static void open(const char *path)
    {
        if (sqlite3_open(path, &Database::handle) != SQLITE_OK)
            sqlite_error();
    }

    static void close()
    {
        if (sqlite3_close(Database::handle) != SQLITE_OK)
            sqlite_error();
    }

    static int getExtensionId(std::string name)
    {
        std::string sql = "SELECT id FROM extensions WHERE name = ?";

        sqlite3_stmt *statement;
        if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, 0) == SQLITE_OK)
        {
            sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(statement) == SQLITE_ROW)
            {
                int id = sqlite3_column_int(statement, 0);
                sqlite3_finalize(statement);

                return id;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            sqlite_error();
            return 0;
        }
    }

    static std::vector<std::string> getExtensionNames()
    {
        std::string sql = "SELECT name FROM extensions WHERE 1";
        std::vector<std::string> names;

        sqlite3_stmt *statement;
        if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, 0) == SQLITE_OK)
        {
            while (sqlite3_step(statement) == SQLITE_ROW)
                names.push_back(std::string((const char *)sqlite3_column_text(statement, 0)));

            sqlite3_finalize(statement);
        }
        else
        {
            sqlite_error();
        }

        return names;
    }

    static std::vector<int> getSessionIds(std::string extensionName)
    {
        std::string sql = "SELECT s.id FROM extensions e LEFT JOIN sessions s ON e.id = s.extension_id WHERE e.name = ? ORDER BY s.id DESC";
        std::vector<int> ids;

        sqlite3_stmt *statement;
        if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, 0) == SQLITE_OK)
        {
            sqlite3_bind_text(statement, 1, extensionName.c_str(), -1, SQLITE_STATIC);

            while (sqlite3_step(statement) == SQLITE_ROW)
                ids.push_back(sqlite3_column_int(statement, 0));

            sqlite3_finalize(statement);
        }
        else
        {
            sqlite_error();
        }

        return ids;
    }

    static std::vector<int> getGenerationIds(std::string extensionName, int sessionId)
    {
        std::string sql = "SELECT g.id FROM extensions e LEFT JOIN sessions s ON e.id = s.extension_id LEFT JOIN generations g ON g.session_id = s.id WHERE e.name = ? AND s.id = ? ORDER BY g.id DESC";
        std::vector<int> ids;

        sqlite3_stmt *statement;
        if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, 0) == SQLITE_OK)
        {
            sqlite3_bind_text(statement, 1, extensionName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(statement, 2, sessionId);

            while (sqlite3_step(statement) == SQLITE_ROW)
                ids.push_back(sqlite3_column_int(statement, 0));

            sqlite3_finalize(statement);
        }
        else
        {
            sqlite_error();
        }

        return ids;
    }

    static int addSession(int extensionId)
    {
        sqlite3_stmt *statement;
        std::string sql = "INSERT INTO sessions(extension_id, created_at, updated_at) VALUES(?, datetime('now'), datetime('now'))";

        if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK)
        {
            sqlite3_bind_int(statement, 1, extensionId);

            if (sqlite3_step(statement) != SQLITE_DONE)
                sqlite_error();
            else
                return sqlite3_last_insert_rowid(Database::handle);
        }
        else
        {
            sqlite_error();
        }
        sqlite3_finalize(statement);

        return 0;
    }

    static void addGeneration(int sessionId, uint8_t *state, int stateSize)
    {
        sqlite3_stmt *statement;
        std::string sql = "INSERT INTO generations(session_id, state, created_at, updated_at) VALUES(?, ?, datetime('now'), datetime('now'))";

        if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, 0) == SQLITE_OK)
        {
            sqlite3_bind_int(statement, 1, sessionId);
            sqlite3_bind_blob(statement, 2, state, stateSize, SQLITE_STATIC);

            if (sqlite3_step(statement) != SQLITE_DONE)
                sqlite_error();
        }
        else
        {
            sqlite_error();
        }
        sqlite3_finalize(statement);
    }
};

sqlite3 *Database::handle = nullptr;

#endif // _DATABASE_H_
