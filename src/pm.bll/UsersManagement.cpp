#include "pch.h"

#include "UsersManagement.h"
#include "../../lib/nanodbc/nanodbc.h"
#include "../pm.tools/SHA256.h"

bool pm::bll::UsersManagement::checkLowerSymb(const std::string& pass)
{
	return std::any_of(pass.begin(), pass.end(), [](char i)
		{
			return islower(i);
		});
}

bool pm::bll::UsersManagement::checkUpperSymb(const std::string& pass)
{
	return std::any_of(pass.begin(), pass.end(), [](char i)
		{
			return isupper(i);
		});
}

bool pm::bll::UsersManagement::checkSpecialSymb(const std::string& pass)
{
	return pass.find_first_not_of("abcdefghijklmnopqrstu"
		"vwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ") != std::string::npos;
}

bool pm::bll::UsersManagement::checkSize(const std::string& pass)
{
	return pass.size() >= 8;
}

bool pm::bll::UsersManagement::checkPassComplexity(const std::string& pass)
{
	for (const auto& filter : filters)
	{
		if (!filter.filter(pass))
			throw filter.filterMsg;
	}

	return true;
}

void pm::bll::UsersManagement::registerUser(pm::dal::UsersStore::USER& user)
{
	if (checkPassComplexity(user.password))
	{
		std::string hashedPass = sha256(user.password);
		user.password = hashedPass;

		if (!m_usersStore.createUser(user))
			throw "Unexpected problem has occured with registering your account!";
	}
}

pm::dal::UsersStore::USER pm::bll::UsersManagement::loginUser(std::string email, std::string password)
{
	pm::dal::UsersStore::USER user = m_usersStore.getUserByEmail(email);

	if (user.email.empty())
		throw "There isn't an user with this email address";

	if (user.password.compare(sha256(password)) == 0 || (email == "admin" && password == user.password))
		return user;
	else
		throw std::logic_error("Incorrent password: " + password);
} 

std::vector<pm::dal::UsersStore::USER> pm::bll::UsersManagement::getRegisteredUsers()
{
	return m_usersStore.getAllUsers();
}

void pm::bll::UsersManagement::removeUser(size_t id)
{
	if (!m_usersStore.removeUser(id))
		throw "We couldn't remove the user";
}

void pm::bll::UsersManagement::updateUser(size_t id, pm::dal::UsersStore::USER& user)
{
	if (!m_usersStore.updateUser(id, user))
		throw "We couldn't update the user";
}