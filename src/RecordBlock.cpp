#include"RecordBlock.hpp"

RecordBlock::RecordBlock() {
	tablename = "";
	prev = next = nullptr;
	dirty = 0;
	locked = 0;
}