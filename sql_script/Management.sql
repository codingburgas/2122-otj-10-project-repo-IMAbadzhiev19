CREATE DATABASE Management
GO

USE Management
GO

--UsersTable
CREATE TABLE [Users] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[FirstName] varchar(31) NOT NULL,
	[LastName] varchar(31) NOT NULL,
	[Email] varchar(101) NOT NULL,
	[Age] tinyint NOT NULL,
	[Password] varchar(301) NOT NULL,
	[CreatedOn] datetime NOT NULL,
	[Admin] bit NOT NULL,
)
GO
--UsersTable

IF NOT EXISTS (SELECT * FROM Users WHERE Id = 1)
BEGIN
	INSERT INTO Users (FirstName, LastName, Email, Age, [Password], CreatedOn, [Admin]) VALUES ('NoFName','NoLName', 'admin', 0, 'adminpass', GETDATE(), 1)
END


--TeamsTable
CREATE TABLE [Teams] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[Title] varchar(31) NOT NULL,
	[CreatedOn] datetime NOT NULL,
	[CreatorId] int CONSTRAINT FK_CreatorId_Teams FOREIGN KEY REFERENCES [Users](Id) ON UPDATE CASCADE,
)
GO
--TeamsTable



--UsersAndTeams
CREATE TABLE [UsersAndTeams] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[UserId] int CONSTRAINT FK_UserId_UT FOREIGN KEY REFERENCES [Users](Id) ON DELETE CASCADE, 
	[TeamId] int CONSTRAINT FK_TeamId_UT FOREIGN KEY REFERENCES [Teams](Id) ON DELETE CASCADE,
	UNIQUE(UserId, TeamId)
)
GO
--UsersAndTeams



--Projects
CREATE TABLE [Projects] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[Title] varchar(31) NOT NULL,
	[Description] varchar(101) NOT NULL,
	[CreatedOn] datetime NOT NULL,
	[CreatorId] int CONSTRAINT FK_CreatorId_Projects FOREIGN KEY REFERENCES [Users](Id) ON UPDATE CASCADE,
)
GO
--Projects



--TeamsAndProjects
CREATE TABLE [TeamsAndProjects] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[TeamId] int CONSTRAINT FK_TeamId_TP FOREIGN KEY REFERENCES [Teams](Id) ON DELETE CASCADE,
	[ProjectId] int CONSTRAINT FK_ProjectId_TP FOREIGN KEY REFERENCES [Projects](Id) ON DELETE CASCADE,
	UNIQUE(TeamId, ProjectId)
)
GO
--TeamsAndProjects



--TasksTable
CREATE TABLE [Tasks] (
	Id int IDENTITY(1, 1) PRIMARY KEY,
	[Title] varchar(31) NOT NULL,
	[Description] varchar(101) NOT NULL,
	[Status] varchar(11) NOT NULL,
	[CreatedOn] datetime NOT NULL,
	[CreatorId] int CONSTRAINT FK_CreatorId_Tasks FOREIGN KEY REFERENCES [Users](Id) ON UPDATE CASCADE,
)
GO
--TasksTable



--ProjectsAndTasks
CREATE TABLE [ProjectsAndTasks] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[ProjectId] int CONSTRAINT FK_ProjectId_PT FOREIGN KEY REFERENCES [Projects](Id) ON DELETE CASCADE,
	[TaskId] int CONSTRAINT FK_TaskId_PT FOREIGN KEY REFERENCES [Tasks](Id) ON DELETE CASCADE,
	UNIQUE(TaskId, ProjectId)
)
Go
--ProjectsAndTasks



--TasksAndUsers
CREATE TABLE [TasksAndUsers] (
	[Id] int IDENTITY(1, 1) PRIMARY KEY,
	[UserId] int CONSTRAINT FK_UserId_PU FOREIGN KEY REFERENCES [Users](Id) ON UPDATE CASCADE,
	[TaskId] int CONSTRAINT FK_TaskId_PU FOREIGN KEY REFERENCES [Tasks](Id) ON DELETE CASCADE,
	UNIQUE(UserId, TaskId)
)
GO
--TasksAndUsers