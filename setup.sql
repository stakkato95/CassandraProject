CREATE KEYSPACE autonomousflight WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };

CREATE TABLE company (
	id int,
	name varchar,
	address varchar,
	PRIMARY KEY (id)
);

insert into company (id, name, address) values (1, 'Runtastic', 'Linz');
insert into company (id, name, address) values (2, 'Blackberry', 'Canada');
insert into company (id, name, address) values (3, 'Microsoft', 'USA');