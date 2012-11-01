CREATE TRIGGER PersonRemovePhones AFTER DELETE ON Person
BEGIN
	DELETE FROM Contact WHERE RefPersonID = old.PersonID;
END
