//
// Created by joshgreaves on 5/9/17.
//

#include "Holodeck.h"
#include "HolodeckSharedmemory.h"

const char HOLODECK_BASE_PATH[] = "/HOLODECK_MEM_";

HolodeckSharedMemory::HolodeckSharedMemory(const std::string& Name, int BufferSize) :
		MemPath(HOLODECK_BASE_PATH + Name), MemSize(BufferSize) {

	#if PLATFORM_WINDOWS
	std::wstring STemp = std::wstring(MemPath.begin(), MemPath.end());
	LPCWSTR WindowsMemPath = STemp.c_str();
	MemFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, this->MemSize, WindowsMemPath);
	MemPointer = static_cast<void*>(MapViewOfFile(MemFile, FILE_MAP_ALL_ACCESS, 0, 0, this->MemSize));
	#elif PLATFORM_LINUX
    MemFile = shm_open(MemPath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
    ftruncate(MemFile, this->memSize);
    MemPointer = static_cast<void*>(mmap(nullptr, this->MemSize, PROT_READ | PROT_WRITE,
                                         MAP_SHARED, memFile, 0));
	#endif
}
