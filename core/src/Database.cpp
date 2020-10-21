#include <sqlite3/sqlite3.h>
#include <cstdio>
#include <string>
#include <vector>
#include <core/Database.h>

template <class T>
std::vector<T> process_result(std::vector<T> &result)
{
    if (result.size() == 1 && result[0] == 0)
        return std::vector<T>();
    else
        return result;
}

static void open(std::string path)
{
    if (sqlite3_open(path.c_str(), &Database::handle) != SQLITE_OK)
        printf("Failed to open database.\n");
}

static void close()
{
    if (sqlite3_close(Database::handle) != SQLITE_OK)
        printf("Failed to close database.\n");
}

static void clearSession(int sessionId)
{
    std::string sql = "DELETE FROM generations WHERE id IN (SELECT g.id FROM sessions s LEFT JOIN generations g ON g.session_id = s.session_id WHERE s.id = ?)";

    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(statement, 1, sessionId);

        if (sqlite3_step(statement) != SQLITE_ROW)
            printf("Failed to clear session.\n");
    }
    else
    {
        printf("Failed to clear session.\n");
    }
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
        printf("Failed to get extension id.\n");
        return 0;
    }
}

static std::vector<std::string> getExtensionNames()
{
    std::string sql = "SELECT name FROM extensions WHERE 1";
    std::vector<std::string> names;

    sqlite3_stmt *statement;
    const char *error;
    if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, &error) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
            names.push_back(std::string((const char *)sqlite3_column_text(statement, 0)));

        sqlite3_finalize(statement);
    }
    else
    {
        printf("Failed to get extension names.\n");
    }

    return names;
}

std::vector<int> Database::getSessionIds(std::string extensionName)
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
        printf("Failed to get session IDs.\n");
    }

    return ids;
}

std::vector<int> Database::getGenerationIds(std::string extensionName, int sessionId)
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
        printf("Failed to get generation IDs.\n");
    }

    return process_result<int>(ids);
}

int Database::addSession(int extensionId)
{
    sqlite3_stmt *statement;
    std::string sql = "INSERT INTO sessions(extension_id, created_at, updated_at) VALUES(?, datetime('now'), datetime('now'))";

    if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(statement, 1, extensionId);

        if (sqlite3_step(statement) != SQLITE_DONE)
            printf("Failed to add new session.\n");
        else
            return sqlite3_last_insert_rowid(Database::handle);
    }
    else
    {
        printf("Failed to add new session.\n");
    }
    sqlite3_finalize(statement);

    return 0;
}

void Database::addGeneration(int sessionId, uint8_t *state, int stateSize)
{
    sqlite3_stmt *statement;
    std::string sql = "INSERT INTO generations(session_id, state, created_at, updated_at) VALUES(?, ?, datetime('now'), datetime('now'))";

    if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(statement, 1, sessionId);
        sqlite3_bind_blob(statement, 2, state, stateSize, SQLITE_STATIC);

        if (sqlite3_step(statement) != SQLITE_DONE)
            printf("Failed to add new generation.\n");
    }
    else
    {
        printf("Failed to add new generation.\n");
    }
    sqlite3_finalize(statement);
}
