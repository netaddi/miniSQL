#include "../src/BufferManager.hpp"

int main(int argc, char const *argv[]) {

    BufferManager bufMan;

    bufMan.getIndexBlock("testTable", "attr1", 0);
    bufMan.getIndexBlock("testTable", "attr1", 0);
    bufMan.getIndexBlock("testTable", "attr1", 0);

    return 0;
}
