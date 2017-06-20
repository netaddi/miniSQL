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

cat_test:
	$(CC) test/cat_test.cpp src/CatalogManager.cpp -o test/cat_test $(Flags)
	./test/cat_test

buf_test:
	$(CC) test/bufManTest.cpp src/BufferManager.cpp src/RecordBlock.cpp -o test/buf_test $(Flags)
	./test/buf_test

bpt_test:
	$(CC) test/bpt_test.cpp -o test/bpt_test $(Flags)
	./test/bpt_test
