drop table student;

create table student 
	( 
		id int unique,
		name varchar(32), 
		score float,
		primary key(id)
	);

create table student 
	( 
		id int unique,
		name varchar(32), 
		score float,
		primary key(id)
	);

create index name_index on student (name);
create index name_index on student (name);
drop index name_index;
drop index name_index;

insert into student 
	values (1, "Wei Jiarong", 59.9);
insert into student 
	values (2, "Yang Xi", 59.8);
insert into student 
	values (3, "Jiang   Ze min", 100); 
insert into student 
	values (4, "HaHa", 120); 

select * from student;
select * from student where id > 1 and score < 110;
select * from student where id > 1 and score < 110 and name != "Yang Xi";
select * from student where name = "Jiang   Ze min";
select * from student where score < 60;
select * from student where score > 60 and name = "Jiang   Ze min";

delete from student where id > 2;
select * from student;
