#include "pch.h"
#include "UserManager.h"

std::string pm::bll::UserManager::hashString(const std::string& str)
{
	std::string hashedString(str);

	std::transform(hashedString.begin(), hashedString.end(), hashedString.begin(), [](char& i)
		{
			return i ^= 15;
		});

	return hashedString;
}

bool pm::bll::UserManager::checkLowerSymb(const std::string& password)
{
	return std::any_of(password.begin(), password.end(), [](char i)
		{
			return islower(i);
		});
}

bool pm::bll::UserManager::checkUpperSymb(const std::string& password)
{
	return std::any_of(password.begin(), password.end(), [](char i)
		{
			return isupper(i);
		});
}

bool pm::bll::UserManager::checkSpecialSymb(const std::string& password)
{
	return password.find_first_not_of("abcdefghijklmnopqrstu"
		"vwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ") != std::string::npos;
}

bool pm::bll::UserManager::checkSize(const std::string& password)
{
	return password.size() >= 8;
}

bool pm::bll::UserManager::checkPasswordComplexity(const std::string& password)
{
	for (const auto& filter : filters)
	{
		if (!filter.filter(password))
			throw filter.filterMessage;
	}

	return true;
}

void pm::bll::UserManager::registerUser(std::string& firstName, std::string& lastName, unsigned short& age,
	std::string& email, std::string& password)
{
	if (checkPasswordComplexity(password))
	{
		pm::types::User user(firstName, lastName, age, email, hashString(password));

		m_userStore.createUser(user);
	}
}

pm::types::User pm::bll::UserManager::loginUser(std::string& email, std::string& password)
{
	pm::types::User foundEmailedUser = m_userStore.getUserByEmail(email);

	if (foundEmailedUser.passwordHash == hashString(password)) return foundEmailedUser;

	throw std::logic_error("Incorrect password: " + password);
}

std::vector<pm::types::User> pm::bll::UserManager::getRegisteredUsers()
{
	return m_userStore.getAllUsers();
}

void pm::bll::UserManager::removeUser(size_t& id)
{
	m_userStore.removeUser(id);
}

void pm::bll::UserManager::updateUser(pm::types::User& user)
{
	m_userStore.updateUser(user);
}