//
// Created by josh on 5/9/17.
//

#include "Holodeck.h"
#include "HolodeckSharedmemory.h"

std::string HOLODECK_BASE_PATH = "/HOLODECK_MEM_";

HolodeckSharedMemory::HolodeckSharedMemory() {}

HolodeckSharedMemory::HolodeckSharedMemory(std::string name, int memSize) :
		memPath(HOLODECK_BASE_PATH + name), memSize(memSize) {

#if PLATFORM_WINDOWS
	std::wstring stemp = std::wstring(memPath.begin(), memPath.end());
	LPCWSTR windowsMemPath = stemp.c_str();
	//LPCWSTR windowsMemPath = *memPath;
	memFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, this->memSize, windowsMemPath);

	memPointer = static_cast<float*>(MapViewOfFile(memFile, FILE_MAP_ALL_ACCESS, 0, 0, this->memSize));
#elif PLATFORM_LINUX
    // Create the memory for the sensors and their mappings
    memFile = shm_open(memPath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
    ftruncate(memFile, this->memSize);
    memPointer = static_cast<float*>(mmap(nullptr, this->memSize, PROT_READ | PROT_WRITE,
                                         MAP_SHARED, memFile, 0));
#endif
}
