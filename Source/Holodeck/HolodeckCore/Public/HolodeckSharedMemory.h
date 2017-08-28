//
// Created by josh on 5/9/17.
//

#pragma once

#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "HideWindowsPlatformTypes.h"
#elif PLATFORM_LINUX
#include <sys/mman.h>
#include <unistd.h>
#endif

class HOLODECK_API HolodeckSharedMemory
{

public:
    explicit HolodeckSharedMemory(std::string name, int mem_size);

	void* get() const {return memPointer;}

	int size() const {return memSize;}

private:
    // The handle to shared memory
    std::string memPath;
    int memSize;

    // The "file" and pointer to the start
#if PLATFORM_WINDOWS
	HANDLE memFile;
#elif PLATFORM_LINUX
    int memFile;
#endif
	void* memPointer;
};

