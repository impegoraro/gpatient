CREATE TABLE Configuration
(
	Key VARCHAR(20) PRIMARY KEY,
	Value VARCHAR(100)
);

# configuration options availables.

insert into Configuration(Key, Value) values('PosX', '0');
insert into Configuration(Key, Value) values('PosY', '0');
insert into Configuration(Key, Value) values('Width', '0');
insert into Configuration(Key, Value) values('Height', '0');
insert into Configuration(Key, Value) values('Maximized', '0');