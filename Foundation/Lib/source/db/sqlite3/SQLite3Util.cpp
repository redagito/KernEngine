#include <cassert>
#include <iostream>

#include "foundation/db/sqlite3/SQLite3Util.h"

bool bindValue(sqlite3_stmt *stmt, int id, const std::string &str)
{
    assert(stmt != nullptr);

	return sqlite3_bind_text(stmt, id, str.data(), static_cast<int>(str.size()),
                                        SQLITE_STATIC) == SQLITE_OK;
}

bool readValue(sqlite3_stmt *stmt, int column, std::string &str)
{
    assert(stmt != nullptr);

    if (sqlite3_column_type(stmt, column) != SQLITE_TEXT)
    {
        return false;
    }
    str = reinterpret_cast<const char *>(sqlite3_column_text(stmt, column));
    return true;
}

bool readValue(sqlite3_stmt *stmt, int column, int &val)
{
    assert(stmt != nullptr);
    if (sqlite3_column_type(stmt, column) != SQLITE_INTEGER)
    {
        return false;
    }
    val = sqlite3_column_int(stmt, column);
    return true;
}

bool readValue(sqlite3_stmt *stmt, int column, double &val)
{
    assert(stmt != nullptr);

    if (sqlite3_column_type(stmt, column) != SQLITE_FLOAT)
    {
        return false;
    }
    val = sqlite3_column_double(stmt, column);
    return true;
}

bool readValue(sqlite3_stmt *stmt, int column, float &val)
{
    assert(stmt != nullptr);

    double temp;
    if (!readValue(stmt, column, temp))
    {
        return false;
    }
    val = static_cast<float>(temp);
    return true;
}

bool prepareStatement(sqlite3 *db, const std::string &sql, sqlite3_stmt **stmt)
{
    assert(db != nullptr);
    assert(stmt != nullptr);

	return sqlite3_prepare_v2(db, sql.data(), static_cast<int>(sql.size()), stmt,
                                         nullptr) == SQLITE_OK;
}