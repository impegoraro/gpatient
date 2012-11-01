/*
 * sql-connection.h
 *
 *  Created on: Oct 20, 2012
 *      Author: ilan
 */

#ifndef SQL_CONNECTION_H_
#define SQL_CONNECTION_H_

#include <string>

enum SqlErrorCodes
{
	SQL_CONNECTION_OPENED,
	SQL_CONNECTION_CLOSED
};

class SqlException
{
protected:
	int m_type;
	std::string m_msg;

	SqlException(int type, const std::string&) throw();
public:
	SqlException() throw();
	SqlException(const std::string&) throw();
	virtual ~SqlException() throw();

	virtual const std::string& what(void) const throw();
	virtual int get_number(void) const throw();
};


class SqlConnectionException : public SqlException
{
public:
	SqlConnectionException(int type, const std::string&) throw();
};

class SqlConnectionClosedException : public SqlConnectionException
{
public:
	SqlConnectionClosedException() throw();
};

class SqlConnectionOpenedException : public SqlConnectionException
{
public:
	SqlConnectionOpenedException() throw();
};


#endif /* SQL_CONNECTION_H_ */
