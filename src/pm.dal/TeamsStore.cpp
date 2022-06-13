#include "pch.h"

#include "TeamsStore.h"

bool pm::dal::TeamsStore::createTeam(pm::dal::TeamsStore::TEAM& team, size_t userId)
{
	std::string query = "INSERT INTO [Teams] "
		"([Title], [CreatedOn], [CreatorId]) "
		"VALUES ('" + team.title + "', GETDATE() , " + std::to_string(userId) + ")";

	db.executeQuery(query);

	query.clear();

	query = "SELECT Id FROM [Teams] WHERE Title = '" + team.title + "'";
	nanodbc::result res = db.getResultFromSelect(query);

	res.next();
	int teamId = res.get<int>(0);

	addUserToTeam(teamId, userId);

	return true;
}