#pragma once
#include "pch.h"
#include "../pm.types/User.h"
#include "../pm.dal/UserStore.h"

namespace pm::bll
{
	struct UserManager
	{
		pm::dal::UserStore m_userStore;

		std::string hashString(const std::string& str);

		static bool checkLowerSymb(const std::string& password);

		static bool checkUpperSymb(const std::string& password);

		static bool checkSpecialSymb(const std::string& password);

		static bool checkSize(const std::string& password);

		struct Filter
		{
			passwordFilter filter;

			std::string filterMessage;
		};

		/*inline static*/ const std::array<Filter, 4> filters =
		{
			checkLowerSymb, "Must have at least 1 lower case symbol",
			checkUpperSymb, "Must have at least 1 upper case symbol",
			checkSpecialSymb, "Must have at least 1 special symbol",
			checkSize, "Must be at least 8 symbols"
		};

		bool checkPasswordComplexity(const std::string& password);

		void registerUser(std::string& firstName, std::string& lastName, unsigned short& age,
			std::string& email, std::string& password);

		pm::types::User loginUser(std::string& email, std::string& password);

		std::vector<pm::types::User> getRegisteredUsers();

		void removeUser(size_t& id);

		void updateUser(pm::types::User& user);
	};
}