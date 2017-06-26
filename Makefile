CC = g++
Flags = -Wall --std=c++17 -g
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
	$(CC) test/bufManTest.cpp src/BufferManager.cpp src/debug.cpp -o test/buf_test $(Flags)
	./test/buf_test

bpt_test:
	$(CC) test/bpt_test.cpp -o test/bpt_test $(Flags)
	./test/bpt_test


rec_test:
	$(CC) test/rec_test.cpp src/RecordManager.cpp src/BufferManager.cpp src/element.cpp src/debug.cpp src/Queryer.cpp -o test/rec_test $(Flags)
	./test/rec_test

ele_test:
	$(CC) test/ele_test.cpp src/debug.cpp -o test/ele_test $(Flags)
	./test/ele_test

que_test:
	$(CC) test/que_test.cpp src/debug.cpp src/Queryer.cpp -o test/que_test $(Flags)
	./test/que_test

api_test:
	$(CC) test/API_test.cpp src/API.cpp \
	src/element.cpp src/RecordManager.cpp src/CatalogManager.cpp src/BufferManager.cpp src/Queryer.cpp\
		-o test/api_test $(Flags)
	./test/api_test
