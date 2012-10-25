/*
 * sql-connection.h
 *
 *  Created on: Oct 20, 2012
 *      Author: ilan
 */

#ifndef SQL_CONNECTION_H_
#define SQL_CONNECTION_H_

#include <exception>
#include <string>

class SqlException : public std::exception
{
protected:
	std::string m_msg;

public:
	SqlException() throw();
	SqlException(const std::string&) throw();
	virtual ~SqlException() throw();

	virtual const std::string& what(void);
};


class SqlConnection : public SqlException
{
};

class SqlConnectionOpen : public SqlConnection
{

};

#endif /* SQL_CONNECTION_H_ */
