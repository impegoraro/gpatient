CREATE TABLE Blood(
    BloodID INTEGER PRIMARY KEY AUTOINCREMENT, 
    Type CHAR(4) CHECK(Type = 'A' OR Type='A+' OR Type='A-' OR Type='B' OR Type='B+' OR Type='B-' OR Type='O' OR Type='O+' OR  Type='O-' OR Type='AB' OR Type='AB+' OR Type='AB-') UNIQUE NOT NULL
);

CREATE TABLE MaritalStatus(MaritalStatusID INTEGER PRIMARY KEY AUTOINCREMENT, Status VARCHAR(15) NOT NULL);

CREATE TABLE CountryCode(Code SMALLINT PRIMARY KEY, Name NVARCHAR(71) NOT NULL);

CREATE TABLE NumberType(NumberTypeID INTEGER PRIMARY KEY AUTOINCREMENT, Type VARCHAR(10) NOT NULL);

CREATE TABLE Contact
(
	ContactID INTEGER PRIMARY KEY AUTOINCREMENT,
	ContactNumber INTEGER NOT NULL,
	RefPersonID INTEGER NOT NULL,
	RefNumberTypeID INTEGER NOT NULL,
	RefCountryCode INTEGER NOT NULL,

	FOREIGN KEY(RefPersonID) REFERENCES Person(PersonID),
	FOREIGN KEY(RefNumberTypeID) REFERENCES NumberType(NumberTypeID),
	FOREIGN KEY(RefCountryCode) REFERENCES Country(Code)
);

CREATE TABLE Person
(	
	PersonID INTEGER PRIMARY KEY AUTOINCREMENT, 
	Name NVARCHAR(71) NOT NULL, 
	Address NVARCHAR(100) NOT NULL,
	Zip VARCHAR(9) NOT NULL,
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

