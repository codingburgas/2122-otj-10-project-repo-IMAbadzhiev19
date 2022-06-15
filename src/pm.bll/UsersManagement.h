#pragma once
#include "pch.h"
#include "../pm.dal/UsersStore.h"


typedef bool(*passwordFilter)(const std::string&);

namespace pm::bll
{
	class UsersManagement
	{
	public:

		pm::dal::UsersStore m_usersStore;

		static bool checkLowerSymb(const std::string& pass);
		static bool checkUpperSymb(const std::string& pass);
		static bool checkSpecialSymb(const std::string& pass);
		static bool checkSize(const std::string& pass);

		bool checkPassComplexity(const std::string& pass);

		void registerUser(pm::dal::UsersStore::USER& user);
		pm::dal::UsersStore::USER& loginUser(std::string email, std::string password);

		std::vector<pm::dal::UsersStore::USER&> getRegisteredUsers();

		void removeUser(size_t id);
		void updateUser(size_t id, pm::dal::UsersStore::USER& user);

	protected:

		struct Filter
		{
			passwordFilter filter;
			std::string filterMsg;
		};

		const std::array<Filter, 4> filters =
		{
			checkLowerSymb, "Must have at least 1 lower case symbol",
			checkUpperSymb, "Must have at least 1 upper case symbol",
			checkSpecialSymb, "Must have at least 1 special symbol",
			checkSize, "Must be at least 8 symbols"
		};

	};
}