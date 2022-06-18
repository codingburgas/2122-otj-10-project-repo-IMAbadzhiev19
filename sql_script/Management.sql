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

INSERT INTO [Users]
(FirstName, LastName, Email, Age, [Password], [CreatedOn], [Admin])
VALUES ('Ivaylo', 'Abadzhiev', 'ivo147@abv.bg', 16, '123456#qW', GETDATE(), 0)
GO

INSERT INTO [Users]
(FirstName, LastName, Email, Age, [Password], [CreatedOn], [Admin])
VALUES ('Zahari', 'Bonev', 'zbonev@gmail.com', 16, 'MilkoNaScena123#', GETDATE(), 0)
GO

INSERT INTO [Users]
(FirstName, LastName, Email, Age, [Password], [CreatedOn], [Admin])
VALUES ('Dimitur', 'Bodurov', 'DTBodurov19@codingburgas.bg', 17, 'Surfist@123', GETDATE(), 0)
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