#include "pch.h"

#include "UsersManagement.h"
#include "../../lib/nanodbc/nanodbc.h"

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