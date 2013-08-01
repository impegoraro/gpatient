CREATE TABLE PersonBk
(	
	PersonID INTEGER PRIMARY KEY AUTOINCREMENT, 
	Name NVARCHAR(71) NOT NULL, 
	Address NVARCHAR(100) NOT NULL,
	Zip1 INTEGER NOT NULL,
	Zip2 INTEGER NOT NULL,
	Location VARCHAR(51) NOT NULL,
	Sex INT2 NOT NULL,
	Height FLOAT NOT NULL,
	Birthday DATE NOT NULL,
	Birthplace VARCHAR(31) NOT NULL,
	Nationality VARCHAR(31) NOT NULL,
	Profession VARCHAR(31) NOT NULL,
	TaxNumber INT UNIQUE NOT NULL,
	Referer VARCHAR(71),
	Email VARCHAR(256) NOT NULL,
	RefBloodTypeID INTEGER NOT NULL,
	RefMaritalStatusID INTEGER NOT NULL, 
	
	FOREIGN KEY (RefMaritalStatusID) REFERENCES MaritalStatus(MaritalStatusID),	
	FOREIGN KEY(RefBloodTypeID) REFERENCES Blood(BloodID) 
);

insert into Personbk select PersonID,Name,Address,Zip1,Zip2,Location,Sex,Height,Birthday,Birthplace,Nationality,Profession,TaxNumber,Referer,Email,RefBloodTypeID,RefMaritalStatusID from person;
drop table Person;

CREATE TABLE Person
(	
	PersonID INTEGER PRIMARY KEY AUTOINCREMENT, 
	Name NVARCHAR(71) NOT NULL, 
	Address NVARCHAR(100) NOT NULL,
	Zip1 INTEGER NOT NULL,
	Zip2 INTEGER NOT NULL,
	Location VARCHAR(51) NOT NULL,
	Sex INT2 NOT NULL,
	Height FLOAT NOT NULL,
	Birthday DATE NOT NULL,
	Birthplace VARCHAR(31) NOT NULL,
	Nationality VARCHAR(31) NOT NULL,
	Profession VARCHAR(31) NOT NULL,
	TaxNumber INT UNIQUE,
	IdentificationCard INT UNIQUE,
	Referer VARCHAR(71),
	Email VARCHAR(256) NOT NULL,
	RefBloodTypeID INTEGER NOT NULL,
	RefMaritalStatusID INTEGER NOT NULL, 
	
	FOREIGN KEY (RefMaritalStatusID) REFERENCES MaritalStatus(MaritalStatusID),	
	FOREIGN KEY(RefBloodTypeID) REFERENCES Blood(BloodID) 
);

insert into Person(PersonID,Name,Address,Zip1,Zip2,Location,Sex,Height,Birthday,Birthplace,Nationality,Profession,TaxNumber,Referer,Email,RefBloodTypeID,RefMaritalStatusID) 
select PersonID,Name,Address,Zip1,Zip2,Location,Sex,Height,Birthday,Birthplace,Nationality,Profession,TaxNumber,Referer,Email,RefBloodTypeID,RefMaritalStatusID from personBk;
drop table PersonBk;