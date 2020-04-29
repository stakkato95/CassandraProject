CREATE KEYSPACE autonomousflight WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };
use autonomousflight;


CREATE TABLE company (
	id int,
	name varchar,
	address varchar,
	PRIMARY KEY (id)
);
insert into company (id, name, address) values (1, 'Runtastic', 'Linz');
insert into company (id, name, address) values (2, 'Blackberry', 'Canada');
insert into company (id, name, address) values (3, 'Microsoft', 'USA');


CREATE TABLE drone (
	companyid int,
	companyname varchar,
	droneid int,
	model varchar,
	firmwareversion varchar,
	PRIMARY KEY (companyid, droneid)
);
insert into drone (companyid, companyname, droneid, model, firmwareversion) values (1, 'Runtastic', 100500, 'ParrotDrone', '1.1.0-beta');
insert into drone (companyid, companyname, droneid, model, firmwareversion) values (1, 'Runtastic', 100501, 'ParrotDrone Xtra', '11.12.0-beta');
insert into drone (companyid, companyname, droneid, model, firmwareversion) values (2, 'Blackberry', 1007, 'ParrotDrone Ultra', '33.1.0-alpha');
insert into drone (companyid, companyname, droneid, model, firmwareversion) values (2, 'Blackberry', 1008, 'ParrotDrone Mega', '99.1.0-gamma');
insert into drone (companyid, companyname, droneid, model, firmwareversion) values (3, 'Microsoft', 12345, 'ParrotDrone Alpha', '88.1.0-omega');