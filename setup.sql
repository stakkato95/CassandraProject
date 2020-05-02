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

CREATE TABLE flight (
	companyid int,
	droneid int,
	flightid int,
	latitude float,
	longitude float,
	elevation float,
	yearmonthday date,
	hourminutesecond time,
	PRIMARY KEY((companyid, droneid, flightid), yearmonthday, hourminutesecond)
) WITH CLUSTERING ORDER BY (yearmonthday DESC);
insert into flight (companyid, droneid, flightid, latitude, longitude, elevation, yearmonthday, hourminutesecond) values (1, 100500, 1, 48.3721216, 14.5227776, 100, '2020-05-01', '08:12:54');
insert into flight (companyid, droneid, flightid, latitude, longitude, elevation, yearmonthday, hourminutesecond) values (1, 100500, 1, 49.3721216, 14.5227776, 101, '2020-05-01', '08:12:55');
insert into flight (companyid, droneid, flightid, latitude, longitude, elevation, yearmonthday, hourminutesecond) values (1, 1005001, 1, 55.3721216, 55.5227776, 100, '2020-05-05', '08:12:58');
insert into flight (companyid, droneid, flightid, latitude, longitude, elevation, yearmonthday, hourminutesecond) values (1, 1005001, 1, 56.3721216, 56.5227776, 101, '2020-05-05', '08:12:59');
insert into flight (companyid, droneid, flightid, latitude, longitude, elevation, yearmonthday, hourminutesecond) values (2, 1007, 1, 60.3721216, 60.5227776, 100, '2020-12-12', '12:12:12');
insert into flight (companyid, droneid, flightid, latitude, longitude, elevation, yearmonthday, hourminutesecond) values (2, 1007, 1, 61.3721216, 61.5227776, 101, '2020-12-12', '12:12:13');
