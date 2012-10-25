/*
 * sql-connection.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: ilan
 */

#include "sql-connection.h"

SqlException::SqlException()
{
}

SqlException::SqlException(const std::string& str) : m_msg(str)
{
}

SqlException::~SqlException()
{
}

