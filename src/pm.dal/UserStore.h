#pragma once
#include "pch.h"
#include "../pm.types/User.h"

namespace pm::dal
{
	struct UserStore
	{
		static int lastId;

		void createUser(pm::types::User& user);

		std::vector<pm::types::User> getAllUsers();

		void removeUser(size_t id);

		void updateUser(pm::types::User user);

		pm::types::User getUserById(size_t id);

		pm::types::User getUserByEmail(std::string email);
	};

	int UserStore::lastId = 0;
}