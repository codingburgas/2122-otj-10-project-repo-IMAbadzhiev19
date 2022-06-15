#pragma once

#include "pch.h"

#include "../pm.consoleApp/Database.h"
#include "../../lib/nanodbc/nanodbc.h"

namespace pm::dal
{
	class UsersStore
	{
	public:

		struct USER
		{
			int id;
			std::string firstName;
			std::string lastName;
			std::string email;
			short age;
			std::string password;
			nanodbc::date createdOn;
			bool admin;
		};

		bool createUser(USER& user);
		bool updateUser(size_t id, USER& user);
		bool removeUser(size_t id);

		USER getUserByEmail(std::string& email);
		USER getUserById(unsigned id);

		std::vector<USER&> getAllUsers();

	private:

		DatabaseManagement db;

	};
}