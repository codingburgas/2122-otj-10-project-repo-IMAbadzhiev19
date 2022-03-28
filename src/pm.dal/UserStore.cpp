#include "pch.h"
#include "UserStore.h"

static std::vector<pm::types::User> users;

void pm::dal::UserStore::createUser(pm::types::User& user)
{
	auto foundUser = std::ranges::find_if(users, [&user](const pm::types::User& user1) -> bool
		{
			return user.email == user1.email;
		});

	if (foundUser != users.end())
	{
		throw std::range_error("User with email: " + user.email + " already exist!");
	}

	user.id = UserStore::lastId;

	users.emplace_back(user);

	++UserStore::lastId;
}

std::vector<pm::types::User> pm::dal::UserStore::getAllUsers()
{
	return std::vector<pm::types::User>(users);
}

void pm::dal::UserStore::removeUser(size_t id)
{
	auto removedEl = std::remove_if(users.begin(), users.end(), [&id](const pm::types::User& user) -> bool
		{
			return user.id == id;
		});

	if (removedEl == users.end()) throw std::range_error("User with id: " + std::to_string(id) + " does not exist!");

	users.erase(removedEl, removedEl + 1);
}

void pm::dal::UserStore::updateUser(pm::types::User user)
{
	pm::types::User foundUser = getUserById(user.id);

	std::replace_if(users.begin(), users.end(), [&foundUser](const pm::types::User& current) -> bool
		{
			return current.id == foundUser.id;
		}, user);
}

pm::types::User pm::dal::UserStore::getUserById(size_t id)
{
	auto foundUser = std::find_if(users.begin(), users.end(), [&id](const pm::types::User& user)-> bool
		{
			return user.id == id;
		});

	if (foundUser == users.end()) throw std::range_error("User with id: " + std::to_string(id) + " does not exist");

	return *foundUser;
}

pm::types::User pm::dal::UserStore::getUserByEmail(std::string email)
{
	auto foundUser = std::find_if(users.begin(), users.end(), [&email](const pm::types::User& user)-> bool
		{
			return user.email == email;
		});

	if (foundUser == users.end()) throw std::range_error("User with email: " + email + " does not exist");

	return *foundUser;
}