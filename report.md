# mini SQL 详细设计报告

姓名 | 学号 | 专业
--- | ---- | ----
杨熙 | 3150105002 | 计算机科学与技术（中加班中方生）

## usage

最终编译结束的程序是minisql。程序提供两种交互方式

### 命令行交互

直接运行`minisql`即可进入交互命令行，执行SQL。

### 执行文件交互

有两种方式执行sql文本文件
 - 将目标文件作为shell中minisql的第一个参数，如：
```bash
./minisql ./testSQL/test.sql
```
 - 在交互模式中执行`exec [filneme]`或`src [filename]`命令同样可以执行该文件中所有的SQL。





## 各模块设计陈述

本组最终完成的miniSQL的**所有**模块均由本人独立完成。因此该报告中将陈述所有模块的设计思路。

### 整体文件架构

`data` 文件夹存放数据库的所有数据，包括表的结构和表中数据的二进制形式。其中`catalog`文件夹存放表的信息，`record`文件夹存放表内容的二进制数据，`index.cat`为所有索引的列表，`table.cat`为所有表的索引。

`src`文件夹包含除`main.cpp`外所有源代码，包括头文件和cpp实现。

`test`文件夹包含各个模块的单元测试程序，在Makefile中有不同的target对应不同模块的单元测试。

`testSQL`中包含最终测试用的SQL脚本和生成压力测试SQL用的jupyter notebook

### 基本数据对象
在`src/element.h`和`src/element.cpp`中定义了miniSQL使用的基本基本数据对象。

它们包括：

`class Attribute`：表中一个Attribute的基本信息。Attribute类包含了类型、名称、大小、是否唯一等基本信息。

`class TableInfo`：一张表的基本信息。包含了表名、主键、所有Attribute信息、单条数据的长度。

`class IndexInfo`：一个索引的基本信息。包括了表、Attribute属性、索引名等。

`class Element`：表中一个基础数据元素，一个Element即为一个数据。Element类中包含了`int`、`string`、`float`三种类型的数据。

Element类通过对构造函数的重载，实现了从不同的数据类型分别生成不同的Element，也可以直接从二进制字节流和类型、大小作为参数生成Element。

Element还重载了`>` `<` `>=` `<=` `==` `!=`等运算符以进行数据之间的比较；重载了`<<`运算符以输出数据。

除此之外，Element类还有以下接口：
```c++
void resize(int newSize)
```
用于改变单个数据元素的尺寸大小
```c++
char * getBitToBuffer()
```
把当前Element的数据内容转换成二进制字节流
```c++
void retriveFromBit(char * sourceBit)
```
从二进制字节流构造新的数据元素。




### Catalog Manager

包含文件：`src/CatalogManager.h` `src/CatalogManager.cpp`

该模块负责读取、写入每张表和索引的基本信息。这些信息以文本形式存放在data文件夹下对应的文件和文件夹中。

以下是接口说明
```c++
string getPrimaryKeyIndexName(TableInfo table);
```
该接口接受一张表信息，返回catlog文件夹中存放该表信息的文件名。
```c++
bool createTableCatalog(TableInfo tableInfo);
```
接受一张表信息，为其创建catalog文件，并加入表列表文件中。
```c++
bool dropTableCatalog(string tableName);
```
通过表名删除表文件。
```c++
bool createIndexCatalog(IndexInfo indexInfo);
```
接受索引信息，将其加入索引列表。
```c++
bool dropIndexCatalog(string indexName);
```
通过索引名删除索引。
```c++
bool dropIndexOfTable(string tableName);
```
删除一张表对应的所有索引。
```c++
bool checkTableExistance(string tableName);
```
检查表是否存在。
```c++
bool checkIndexExistanceWithName(string indexName);
```
通过索引名检查索引是否存在。
```c++
bool checkIndexExistanceWithAttr(string tableName, string attributeName);
```
通过表名和属性名检查索引是否存在。
```c++
bool checkAttributeUnique(string tableName, string attributeName);
```
检查表的某一属性是否唯一。
```c++
TableInfo getTableInfo(string tableName);
```
通过表明获取表信息。
```c++
IndexInfo getIndexInfo(string indexName);
```
通过索引名获取索引信息。~~实际上这个接口没用= =~~
```c++
IndexInfo getIndexInfo(string tableName, string attributeName);
```
通过表名和属性名获取索引信息。
```c++
map<string, TableInfo> initializeTables();
```
初始化数据库时读取所有表，返回表名和表详细信息的map。
```c++
map<string, IndexInfo> initializeIndexes();
```
初始化数据库时读取所有索引，返回索引名和索引信息的map。

在以上接口中，所有返回`bool`类型的接口，均会在执行成功时返回`true`，失败返回`false`。





### 查询有关的类

包含文件：`src/Queryer.h` `src/Queryer.cpp`

这一部分主要实现了用于对数据进行查询的类。首先实现一个用于查询的抽象基类`QueryBase`，该类提供一个纯虚函数`bool match(Record record)`用于检查某一条record是否符合查询的条件。接下来实现了三个继承了该基类的模板类`SingleQuery` `RangeQuery` `infinityRangeQuery`，分别进行等值查询、区间查询和无限区间查询。这三个类分别重载了`match`函数，用于检查传入的record是否符合当前查询条件。





### Buffer Manager

包含文件：`src/BufferManager.h` `src/BufferManager.cpp`

该部分实现了三个类：`BufferPage` `BufferPool` `BufferManager`。这三个类的抽象层级依次递增。

`BufferPage`为最底层的单页Buffer，大小为4KB，也是Buffer写入文件的最终单位。在写入硬盘的4KB block文件中，前4个字节代表这个block实际使用的数据数量，后面的4092个字节为实际的二进制buffer。

`BufferPool`管理一张表使用的所有Buffer，提供表级别的Buffer抽象，同时管理的底层的缓存页。每张表对应一个BufferPool对象，对该表的所有操作都通过这一个BufferPool对象操作。

`BufferManager`提供数据库级别的Buffer抽象，也是BufferManager模块的顶层接口。该模块建立了表名到BufferPool对象之间的映射，对每一张表都维护一个BufferPool。
这三个模块之间的关系如图：

BufferManager的接口说明：
```c++
void createBufferForTable(TableInfo & table);
```
该接口为一张新的表分配缓存池。
```c++
void dropTableBuffer(string tableName);
```
该接口在删除表时调用，释放缓存池的内存并删除硬盘上的Buffer文件。
```c++
int insertIntoTable(string table, char * newData);
```
该接口插入一个新的条目到表对应的buffer中，传入的是record的二进制字节流。返回的数据为该条目在buffer中的偏移量，用于存放于索引中。
```c++
char * queryTableWithOffset(string table, int offset);
```
该接口接收表名和对应记录的偏移量，返回对应记录的二进制字节。该接口用于有索引存在时，从索引的偏移量中读取条目。
```c++
char * queryCompleteTable(string table);
```
该接口接收一个表名，返回该表全部的二进制字节。
```c++
int getTableBufferSize(string table);
```
该接口接受表名，返回该表的Buffer的总大小。
```c++
bool deleteFromTableWithOffset(string table, int offset);
```
该接口接受表名和偏移量作为参数，删除表中偏移位置对应的记录。
```c++
bool deleteFromTableWithCheckFunc(string table, function<bool (char *)> checkDeletionFunction);
```
该接口接受表名和一个检查函数作为参数，对该表所对应的所有条目的二进制字节执行检查函数，如果检查函数返回true，则删除该条目。
```c++
bool writeBackAll();
```
以上的所有缓存操作均在内存执行，该接口将所有缓存写入硬盘。
```c++
bool writeBackTable(string table);
```
该函数接收表名，将该表的还粗写回硬盘。
```c++
bool reloadTable(string table);
```
该函数从硬盘中读取对应表的所有block到内存中，用于在初始化数据库时执行。

与Catalog Manager相同，以上操作中返回`bool`类型的函数会在执行成功时返回`true`，否则返回`false`。





### Record Manager

包含文件：`src/RecordManager.h` `src/RecordManager.cpp` `src/Record.h`

其中`src/Record.h`中定义的`Record`类实现了数据库的一条记录。Record类的构造函数可以通过Record本身的表信息加Record中元素的列表进行构造，同时重载了`[]`运算符以获取某一项属性的值，重载了`<<`运算符以输出一条记录的中的全部内容。

而Record Manager对Buffer Manager提供的二进制字节信息进行了解释和封装，提供了对数据库二进制数据进行插入、删除、读取的接口。具体接口如下：

```c++
void initTable(TableInfo table);
```
该接口为已存在的表或新表申请Buffer。
```c++
void dropTable(string tableName);
```
该接口删除已存在的表的信息。
```c++
int insert(Record record);
```
该接口插入一条新纪录，接收参数为Record对象，其中Record类中已经包含了插入的表的信息。
```c++
vector<Record> queryWithOffset(TableInfo table, vector<int> offsets);
```
通过偏移量的列表查询表中的记录，返回每个偏移量对应记录的Record类的vector。该函数用于在有索引时调用。
```c++
vector<Record> queryWithCondition(TableInfo table, vector<QueryBase *> querys);
```
该接口传入待查询的表和查询条件指针的vector，遍历表中的所有记录，返回所有符合所有查询条件的记录。
```c++
bool deleteWithOffset(string table, vector<int> offsets);
```
该接口通过指定的偏移量删除对应记录。
```c++
int deleteWithCondition(TableInfo table, vector<QueryBase *> querys);
```
该接口传入查询条件指针的vector，对每一条记录进行判定，如果符合所有条件则将其删除。
```c++
void writeBackAll();
```
该接口是对BufferManager写回硬盘的进一步封装。


#Index Manager
包含文件：`src/IndexManager.h` `src/IndexManager.cpp` `src/BPlusTree.h`

在`src/BPlusTree.h`中实现了一个B+树的模板类，该类接收key类型K、value类型V和B+树的order M作为模板参数，其中M有默认值3.

Index Manager 对B+树进行的封装，同时建立了索引到对应的B+树之间的映射。该类提供以下接口：

```c++
void createIndex(IndexInfo indexInfo);
```
根据索引信息创建新的B+树索引并建立映射。
```c++
void dropIndex(IndexInfo indexInfo);
```
删除一个索引
```c++
void dropAllIndexFromTable(string tableName);
```
删除某张表下的所有索引
```c++
void insertIntoIndex(IndexInfo index, Element value, int offset);
```
向索引中插入信息
```c++
int queryFromIndex(string table, string attr, Element * e);
```
根据Element在索引中建立信息。
```c++
void deleteFromIndex(IndexInfo index, Element value);
```
删除索引中的某个值。

###  顶层API

包含文件：`src/API.h` `src/API.cpp`

API是对底层所有模块的统一封装，提供了对数据库支持的所有操作的抽象，将底层操作映射到顶层。

接口说明：
```c++
void writeBackAll();
```
写回硬盘
```c++
void createTable(TableInfo newTable);
```
创建表
```c++
void deleteTable(string tableName);
```
删除表
```c++
void createIndex(IndexInfo newIndex);
```
创建索引
```c++
void deleteIndex(string indexName);
```
删除索引
```c++
void insertInto(string table, vector<Element *> elements);
```
插入数据
```c++
void deleteFrom(string table, vector<QueryBase*> Querys);
```
删除数据
```c++
void selectFrom(string table, vector<string> columns, vector<QueryBase*> Querys);
```
查询数据
```c++
int getAttributeType(string table, string attr);
```
获取某个表中某个属性的类型，该接口用于在解释器中生成
```c++
int getAttributeSize(string table, string attr);
```
获取某个表中某个Attribute的字节长度。

### SQL解释器

文件包括：`src/parser.h` `src/parser.cpp`

解释器使用类似于状态机的方式对SQL语言进行解析，并提供从shell接收标准输入的接口，即miniSQL的主循环。

对每个执行的语句，
