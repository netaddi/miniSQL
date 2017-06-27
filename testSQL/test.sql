create table classroom
	(
	building    char(15),
	room_number char(7),
	capacity	 int,
	primary key (room_number)
	);

create table department
	(
	id int,
	dept_name char(20),
	building char(15),
	budget  int,
	primary key (id)
	);
