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

bool pm::dal::TeamsStore::updateTeam(size_t teamId, TEAM& team, size_t userId)
{
	std::string query = "UPDATE [Teams]"
		"SET Title = '" + team.title + "' WHERE Id = " + std::to_string(teamId);

	db.executeQuery(query);
	return true;
}

bool pm::dal::TeamsStore::removeTeam(size_t teamId)
{
	std::string query = "DELETE FROM [Teams] WHERE Id = " + std::to_string(teamId);
	db.executeQuery(query);

	return true;
}

bool pm::dal::TeamsStore::addUserToTeam(size_t teamId, size_t userId)
{
	std::string query = "INSERT INTO [UsersAndTeams] "
		"(UserId, TeamId) "
		"VALUES (" + std::to_string(userId) + "," + std::to_string(teamId) + ")";

	db.executeQuery(query);
	return true;
}