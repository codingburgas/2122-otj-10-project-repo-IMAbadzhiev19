#include"Database.h"

using namespace std;

nanodbc::connection DatabaseManagement::getConn()
{
	return this->conn;
}

nanodbc::result DatabaseManagement::getResultFromSelect(string query)
{
	nanodbc::result res = execute(this->conn, query);
	return res;
}

void DatabaseManagement::executeQuery(string query)
{
	nanodbc::execute(this->conn, query);
}