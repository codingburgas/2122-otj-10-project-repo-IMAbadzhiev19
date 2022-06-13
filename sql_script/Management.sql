CREATE DATABASE Management

USE Management

CREATE TABLE [Users] (
	Id int IDENTITY(1, 1) PRIMARY KEY,
	[FirstName] varchar(31) NOT NULL,
	[LastName] varchar(31) NOT NULL,
	[Email] varchar(101) NOT NULL,
	[Age] tinyint NOT NULL,
	[Password] varchar(301) NOT NULL,
	[CreatedOn] datetime NOT NULL,
	[Admin] tinyint NOT NULL,
)

INSERT INTO [Users]
(FirstName, LastName, Email, Age, [Password], [CreatedOn], [Admin])
VALUES ('Ivaylo', 'Abadzhiev', 'ivo147@abv.bg', 16, '123456#qW', GETDATE(), 0)

INSERT INTO [Users]
(FirstName, LastName, Email, Age, [Password], [CreatedOn], [Admin])
VALUES ('Zahari', 'Bonev', 'zbonev@gmail.com', 16, 'MilkoNaScena123#', GETDATE(), 0)

INSERT INTO [Users]
(FirstName, LastName, Email, Age, [Password], [CreatedOn], [Admin])
VALUES ('Dimitur', 'Bodurov', 'DTBodurov19@codingburgas.bg', 17, 'Surfist@123', GETDATE(), 0)

CREATE TABLE [Teams] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[Title] varchar(31) NOT NULL,
	[CreatedOn] datetime NOT NULL,
	[CreatorId] int FOREIGN KEY REFERENCES [Users](Id) ON UPDATE CASCADE --ON DELETE CASCADE,
)

CREATE TABLE [UsersAndTeams] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[UserId] int FOREIGN KEY REFERENCES [Users](Id), 
	[TeamId] int FOREIGN KEY REFERENCES [Teams](Id),
	UNIQUE(UserId, TeamId)
)

INSERT INTO [Teams]
([Title], [CreatedOn], [CreatorId])
VALUES ('Team1', GETDATE(), 2)

INSERT INTO [Teams]
([Title], [CreatedOn], [CreatorId])
VALUES ('Team2', GETDATE(), 1)

INSERT INTO [UsersAndTeams]
(UserId, TeamId)
VALUES (2, 1)

INSERT INTO [UsersAndTeams]
(UserId, TeamId)
VALUES (1, 1)

INSERT INTO [UsersAndTeams]
(UserId, TeamId)
VALUES (1, 2)

INSERT INTO [UsersAndTeams]
(UserId, TeamId)
VALUES (3, 2)

SELECT * FROM Teams

SELECT * FROM [Users] WHERE Email = 'ivo147@abv.bg'

DELETE FROM [Teams] WHERE Id = 3

SELECT Users.Id, Users.FirstName, Users.lastName, Users.Email, Users.Age, Users.[Password], Users.CreatedOn, Users.[Admin] FROM Users, Teams, UsersAndTeams WHERE (UserId = Users.id) AND (TeamId = Teams.Id) AND (TeamId = 3)
SELECT Teams.Title, YEAR(Teams.CreatedOn), MONTH(Teams.CreatedOn) FROM Users, Teams, UsersAndTeams WHERE (UserId = Users.id) AND (TeamId = Teams.Id) AND (TeamId = 1)
SELECT Teams.Title, YEAR(Teams.CreatedOn), MONTH(Teams.CreatedOn) FROM Users, Teams, UsersAndTeams WHERE (UserId = Users.id) AND (TeamId = Teams.Id) AND (TeamId = 1) AND (YEAR(Teams.CreatedOn) = 2022)