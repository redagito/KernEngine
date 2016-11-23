#pragma once

#include <string>

#include <sqlite3.h>

/**
* \brief Utility wrapper for easy string binding to sqlite3 prepared statements.
* \return False on error and true otherwise.
* \param stmt The prepared statement.
* \param id Id of the parameter in the statement.
* \param text Text to be bound.
*/
bool bindValue(sqlite3_stmt *stmt, int id, const std::string &text);

/**
* \brief Reads string into value from statement.
* \return False on error or type mismatch and true otherwise.
* Performs additional type and error checking.
*/
bool readValue(sqlite3_stmt *stmt, int column, std::string &value);

/**
* \brief Reads integer into value from statement.
* \return False on error or type mismatch and true otherwise.
* Performs additional type and error checking.
*/
bool readValue(sqlite3_stmt *stmt, int column, int &value);

/**
* \brief Reads double into value from statement.
* \return False on error or type mismatch and true otherwise.
* Performs additional type and error checking.
*/
bool readValue(sqlite3_stmt *stmt, int column, double &value);

/**
* \brief Reads float into value from statement.
* \return False on error or type mismatch and true otherwise.
* Performs additional type and error checking.
*/
bool readValue(sqlite3_stmt *stmt, int column, float &value);

/**
* \brief Utility function for easy prepared statement creation.
* \return False on error and true otherwise.
*/
bool prepareStatement(sqlite3 *db, const std::string &sql, sqlite3_stmt **stmt);