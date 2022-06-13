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