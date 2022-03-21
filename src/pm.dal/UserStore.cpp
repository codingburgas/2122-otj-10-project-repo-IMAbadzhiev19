#include "pch.h"
#include "UserStore.h"

#include "../pm.types/User.h"
#include<vector>

static std::vector<pm::types::User> users;

void pm::dal::UserStore::create(pm::types::User user)
{
}

std::vector<pm::types::User> pm::dal::UserStore::getAll()
{
	return std::vector<pm::types::User>();
}

void pm::dal::UserStore::remove(size_t id)
{
}

void pm::dal::UserStore::update(pm::types::User user)
{
}

pm::types::User pm::dal::UserStore::getById(size_t id)
{
	return pm::types::User();
}

pm::types::User pm::dal::UserStore::getByEmail(std::string email)
{
	return pm::types::User();
}
