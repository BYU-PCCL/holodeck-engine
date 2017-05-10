//
// Created by josh on 5/9/17.
//

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <map>
#include "SubscriberInfo.h"
#include <sys/stat.h>
#include <sys/types.h>

#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_LINUX
#include <sys/mman.h>
#include <unistd.h>
#endif

class HOLODECK_API HolodeckSharedMemory
{

public:
	HolodeckSharedMemory();
    HolodeckSharedMemory(FString name, int memSize, int mapSize);

    void clear();

    bool containsKey(const FString &agentName, const FString &name);

    FString get(const FString &agentName, const FString &name);

    void subscribe(const FString &agentName, const FString &name, int size);

    void set(const FString &agentName, const FString &name, char* data);

    FString getData() const;
    FString getMappings() const;

private:
    // The handle to shared memory
    FString memPath;
    FString mapPath;
    int memSize;
    int mapSize;

    // The "file" and pointer to the start
#if PLATFORM_WINDOWS
	HANDLE memFile;
	HANDLE mapFile;
#elif PLATFORM_LINUX
    int memFile;
    int mapFile;
#endif
	char* memPointer;
	char* mapPointer;

    int mapCurrentLength;
    int memLastIndex;
    std::map<FString, SubscriberInfo> subscribedItems;

    int getLength();
    void updateSubscribedMap();
};

