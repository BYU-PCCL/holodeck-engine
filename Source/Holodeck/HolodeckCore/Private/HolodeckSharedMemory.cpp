//
// Created by joshgreaves on 5/9/17.
//

#include "Holodeck.h"
#include "HolodeckSharedMemory.h"

const char HOLODECK_BASE_PATH[] = "/HOLODECK_MEM";

HolodeckSharedMemory::HolodeckSharedMemory(const std::string& Name, unsigned int BufferSize, const std::string& UUID) :
		MemPath(HOLODECK_BASE_PATH + UUID + "_" + Name), MemSize(BufferSize) {

	#if PLATFORM_WINDOWS
	std::wstring STemp = std::wstring(MemPath.begin(), MemPath.end());
	LPCWSTR WindowsMemPath = STemp.c_str();
	MemFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, this->MemSize, WindowsMemPath);
	MemPointer = static_cast<void*>(MapViewOfFile(MemFile, FILE_MAP_ALL_ACCESS, 0, 0, this->MemSize));
	#elif PLATFORM_LINUX
    MemFile = shm_open(MemPath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
    ftruncate(MemFile, this->MemSize);
    MemPointer = static_cast<void*>(mmap(nullptr, this->MemSize, PROT_READ | PROT_WRITE,
                                         MAP_SHARED, MemFile, 0));
	#endif
}
