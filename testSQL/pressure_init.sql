drop table pressure;
create table pressure 
	( 
		id int,
		hash varchar(32), 
		floatId float,
		primary key(id)
	);
insert into pressure values (-1, "00000000000000000000000000000000", -0.1);
select * from pressure;