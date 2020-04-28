CREATE KEYSPACE autonomousflight WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 3 };

CREATE TABLE company (
	company int
);