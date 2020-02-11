#include "stdafx.h"
#include "proc.h"


uintptr_t FindDMAAddr(uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		// read the current value of addr into addr
		addr = *(uintptr_t*)addr;
		// add offset of that memory address
		addr += offsets[i];
	}
	return addr;
}
