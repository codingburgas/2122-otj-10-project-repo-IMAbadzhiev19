#pragma once
#include "pch.h"

typedef bool(*passwordFilter)(const std::string&);

namespace pm::types
{
	struct User
	{
		size_t id;
		std::string firstName;
		std::string lastName;
		std::string email;
		unsigned short age;
		std::string passwordHash;
		std::time_t createdOn;

		User(std::string m_firstName, std::string m_lastName, unsigned short m_age,
			std::string m_email, std::string m_passwordHash)
			: firstName(m_firstName), lastName(m_lastName), age(m_age), email(m_email),
			passwordHash(m_passwordHash)
		{
		}
	};
}