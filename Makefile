CC = g++
Flags = -Wall --std=c++11
finalFilename = minisql
objFiles = main.o parser.o
srcDir = src/

final: $(objFiles)
	$(CC) $(objFiles) -o $(finalFilename)

main.o : main.cpp src/utils.h
	$(CC) main.cpp -c $(Flags)

parser.o : src/parser.h src/parser.cpp
	$(CC) src/parser.cpp -c $(Flags)

clean:
	rm *.o