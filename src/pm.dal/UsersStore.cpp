#include"pch.h"
#include"UsersStore.h"

bool pm::dal::UsersStore::createUser(pm::dal::UsersStore::USER& user)
{
	std::string query = "INSERT INTO [Users] "
		"([FirstName], [LastName], [Email], [Age], [Password], [CreatedOn], [Admin]) "
		"VALUES ('" + user.firstName + "', '" + user.lastName + "', '" + user.email +
		"', " + std::to_string(user.age) + ", '" + user.password + "', GETDATE(), 0)";

	db.executeQuery(query);
	return true;
}

bool pm::dal::UsersStore::updateUser(size_t id, pm::dal::UsersStore::USER& user)
{
	std::string query = "UPDATE [Users] "
		"SET FirstName = '" + user.firstName + "', LastName = '" + user.lastName + "', Email = '" + user.email +
		"', Age = " + std::to_string(user.age) + ", Admin = " + std::to_string(user.admin) +
		"WHERE Id = " + std::to_string(id);

	db.executeQuery(query);
	return true;
}

bool pm::dal::UsersStore::removeUser(size_t id)
{
	std::string query = "DELETE FROM [Users] WHERE Id = " + std::to_string(id);

	db.executeQuery(query);
	return true;
}

pm::dal::UsersStore::USER pm::dal::UsersStore::getUserByEmail(std::string& email)
{
	std::string query = "SELECT * FROM [Users] WHERE Email = '" + email + "'";
	nanodbc::result res = db.getResultFromSelect(query);

	pm::dal::UsersStore::USER usr;

	res.next();

	usr.id = res.get<int>(0);
	usr.firstName = res.get<std::string>(1);
	usr.lastName = res.get<std::string>(2);
	usr.email = res.get<std::string>(3);
	usr.age = res.get<int>(4);
	usr.password = res.get<std::string>(5);
	usr.createdOn = res.get<nanodbc::date>(6);
	usr.admin = res.get<bool>(7);

	return usr;
}

pm::dal::UsersStore::USER pm::dal::UsersStore::getUserById(unsigned id)
{
	std::string query = "SELECT * FROM [Users] WHERE Id = " + std::to_string(id);
	nanodbc::result res = db.getResultFromSelect(query);

	pm::dal::UsersStore::USER usr;

	res.next();

	usr.id = res.get<int>(0);
	usr.firstName = res.get<std::string>(1);
	usr.lastName = res.get<std::string>(2);
	usr.email = res.get<std::string>(3);
	usr.age = res.get<int>(4);
	usr.password = res.get<std::string>(5);
	usr.createdOn = res.get<nanodbc::date>(6);
	usr.admin = res.get<bool>(7);

	return usr;
}