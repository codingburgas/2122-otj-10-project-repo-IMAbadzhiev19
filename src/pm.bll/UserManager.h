#pragma once
#include"../pm.dal/UserStore.h"
#include"../pm.types/User.h"

namespace pm::bll
{
	struct UserManager
	{
		pm::dal::UserStore m_userStore;
		std::string hashString(std::string str);

		void registerUser(std::string firstName, std::string lastName, uint8_t age, std::string email, std::string password);

		pm::types::User loginUser(std::string username, std::string password);
		std::vector<pm::types::User> getRegisteredUsers();

		void removeUser(size_t id);
		void updateUser(pm::types::User);
	};
}