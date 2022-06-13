#pragma once

#include "pch.h"

#include "../pm.consoleApp/Database.h"
#include "../../lib/nanodbc/nanodbc.h"

namespace pm::dal
{
	class TeamsStore
	{
	public:

		struct TEAM
		{
			int id;
			std::string title;
			nanodbc::date createdOn;
			int creatorId;
		};

		bool createTeam(TEAM& team, size_t userId);
		bool updateTeam(size_t teamId, TEAM& team, size_t userId);
		bool removeTeam(size_t teamId);

		bool addUserToTeam(size_t teamId, size_t userId);
		bool removeFromTeam(size_t teamId, size_t userId);

	private:

		DatabaseManagement db;

	};
}