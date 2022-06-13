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