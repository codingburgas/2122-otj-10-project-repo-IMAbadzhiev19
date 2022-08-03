#pragma once

#include "../../lib/nanodbc/nanodbc.h"

class DatabaseManagement
{
public:

	DatabaseManagement() : conn("Driver={ODBC Driver 17 for SQL Server};Server=.\\SQLExpress;Database=Management;Trusted_Connection=yes;") { }

	nanodbc::connection getConn();

	nanodbc::result getResultFromSelect(std::string query);
	void executeQuery(std::string query);


private:

	nanodbc::connection conn;

};