/*
 * sql-connection.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: ilan
 */

#include "sql-connection.h"

SqlException::SqlException() throw() : m_type(-1), m_msg("Sql Connection Exception")
{
}

SqlException::SqlException(const std::string& str) throw() : m_type(-1), m_msg(str)
{
}

SqlException::SqlException(int type, const std::string& str) throw() : m_type(type), m_msg(str)
{
}

SqlException::~SqlException() throw()
{
}

int SqlException::get_number(void) const throw()
{
	return m_type;
}

const std::string& SqlException::what(void) const throw()
{
	return m_msg;
}

SqlConnectionException::SqlConnectionException(int type, const std::string& msg) throw() : SqlException(type, msg)
{
}


SqlConnectionClosedException::SqlConnectionClosedException() throw() :
		SqlConnectionException(SQL_CONNECTION_CLOSED, "Impossível fechar a ligação a base de dados, já se encontra fechada.")
{
}

SqlConnectionOpenedException::SqlConnectionOpenedException() throw() :
		SqlConnectionException(SQL_CONNECTION_OPENED, "Impossível abrir a base de dados porque já se encontra aberta.")
{
}
