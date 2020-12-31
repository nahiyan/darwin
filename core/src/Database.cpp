#include <sqlite3/sqlite3.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <core/Database.h>

using namespace Core;

sqlite3 *Database::handle = nullptr;

void Database::open(std::string path)
{
    if (sqlite3_open(path.c_str(), &Database::handle) != SQLITE_OK)
        printf("Failed to open database.\n");
}

void Database::close()
{
    if (sqlite3_close(Database::handle) != SQLITE_OK)
        printf("Failed to close database.\n");
}

void Database::clearSession(int sessionId)
{
    if (Database::getGenerationIds(sessionId).size() > 0)
    {
        std::string sql = "DELETE FROM generations WHERE id IN (SELECT g.id FROM sessions s LEFT JOIN generations g ON g.session_id = s.id WHERE s.id = ?);";

        sqlite3_stmt *statement;
        if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK)
        {
            sqlite3_bind_int(statement, 1, sessionId);

            sqlite3_step(statement);
        }
        else
        {
            printf("Failed to clear session.\n");
        }
    }
}

uint8_t *Database::getGenerationState(int generationId)
{
    std::string sql = "SELECT state FROM generations WHERE id = ?";

    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(statement, 1, generationId);
        if (sqlite3_step(statement) == SQLITE_ROW)
        {
            auto blobSize = sqlite3_column_bytes(statement, 0);
            auto blob = new uint8_t[blobSize];
            std::memcpy(blob, sqlite3_column_blob(statement, 0), blobSize);

            return blob;
        }
        else
        {
            printf("Failed to get generation state.\n");
        }

        sqlite3_finalize(statement);
    }
    else
    {
        printf("Failed to get generation state.\n");
    }

    return nullptr;
}

int Database::getExtensionId(std::string name)
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

std::vector<std::string> Database::getExtensionNames()
{
    std::string sql = "SELECT name FROM extensions WHERE 1";
    std::vector<std::string> names;

    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
            names.push_back(std::string((const char *)sqlite3_column_text(statement, 0)));

        sqlite3_finalize(statement);
    }
    else
    {
        printf("Failed to get extension names.\n");
    }

    if (names.size() == 1 && names[0] == "")
        names.clear();
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

    if (ids.size() == 1 && ids[0] == 0)
        ids.clear();

    return ids;
}

std::vector<int> Database::getGenerationIds(int sessionId)
{
    std::string sql = "SELECT g.id FROM extensions e LEFT JOIN sessions s ON e.id = s.extension_id LEFT JOIN generations g ON g.session_id = s.id WHERE s.id = ? ORDER BY g.id DESC";
    std::vector<int> ids;

    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(Database::handle, sql.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(statement, 1, sessionId);

        while (sqlite3_step(statement) == SQLITE_ROW)
            ids.push_back(sqlite3_column_int(statement, 0));

        sqlite3_finalize(statement);
    }
    else
    {
        printf("Failed to get generation IDs.\n");
    }

    if (ids.size() == 1 && ids[0] == 0)
        ids.clear();

    return ids;
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
