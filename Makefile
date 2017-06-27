CC = g++
Flags = -Wall --std=c++17 -g
finalFilename = minisql
objFiles = main.o BufferManager.o CatalogManager.o element.o parser.o IndexManager.o \
			Queryer.o Record.o RecordManager.o API.o
srcDir = src/

final: $(objFiles)
	$(CC) $(objFiles) -o $(finalFilename)
	# rm *.o

main.o : main.cpp
	$(CC) main.cpp -c $(Flags)

API.o : src/API.cpp src/API.h
	$(CC) src/API.cpp -c $(Flags)

parser.o : src/parser.h src/parser.cpp
	$(CC) src/parser.cpp -c $(Flags)

BufferManager.o : src/BufferManager.h src/BufferManager.cpp
	$(CC) src/BufferManager.cpp -c $(Flags)

CatalogManager.o : src/CatalogManager.h src/CatalogManager.cpp
	$(CC) src/CatalogManager.cpp -c $(Flags)

element.o : src/element.h src/element.cpp
	$(CC) src/element.cpp -c $(Flags)

IndexManager.o : src/IndexManager.h src/IndexManager.cpp
	$(CC) src/IndexManager.cpp -c $(Flags)

Queryer.o : src/Queryer.h src/Queryer.cpp
	$(CC) src/Queryer.cpp -c $(Flags)

Record.o : src/Record.h src/Record.cpp
	$(CC) src/Record.cpp -c $(Flags)

RecordManager.o : src/RecordManager.h src/RecordManager.cpp
	$(CC) src/RecordManager.cpp -c $(Flags)

clean:
	rm *.o src/*.gch


# ==========================================
#  Unit tests
# ==========================================

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
