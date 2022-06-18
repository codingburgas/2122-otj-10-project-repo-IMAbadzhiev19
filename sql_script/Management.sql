CREATE DATABASE Management
GO

USE Management
GO

CREATE TABLE [Users] (
	Id int IDENTITY(1, 1) PRIMARY KEY,
	[FirstName] varchar(31) NOT NULL,
	[LastName] varchar(31) NOT NULL,
	[Email] varchar(101) NOT NULL,
	[Age] tinyint NOT NULL,
	[Password] varchar(301) NOT NULL,
	[CreatedOn] datetime NOT NULL,
	[Admin] bit NOT NULL,
)
GO

CREATE TABLE [Teams] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[Title] varchar(31) NOT NULL,
	[CreatedOn] datetime NOT NULL,
	[CreatorId] int FOREIGN KEY REFERENCES [Users](Id) ON UPDATE CASCADE,
)
GO


CREATE TABLE [UsersAndTeams] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[UserId] int FOREIGN KEY REFERENCES [Users](Id) ON DELETE CASCADE, 
	[TeamId] int FOREIGN KEY REFERENCES [Teams](Id) ON DELETE CASCADE,
	UNIQUE(UserId, TeamId)
)
GO

SELECT * FROM Users