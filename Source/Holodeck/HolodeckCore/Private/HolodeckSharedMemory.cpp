//
// Created by josh on 5/9/17.
//

#include "Holodeck.h"
#include "HolodeckSharedmemory.h"

HolodeckSharedMemory::HolodeckSharedMemory() {}

HolodeckSharedMemory::HolodeckSharedMemory(FString name, int memSize, int mapSize) {
	memPath = "/HOLODECK_MEM_" + name;
	mapPath = "/HOLODECK_MAP_" + name;
	this->memSize = memSize;
	this->mapSize = mapSize;

#if PLATFORM_WINDOWS
	//std::wstring stemp = std::wstring(memPath.begin(), memPath.end());
	//LPCWSTR windowsMemPath = stemp.c_str();
	LPCWSTR windowsMemPath = *memPath;
	memFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, this->memSize, windowsMemPath);
	//stemp = std::wstring(mapPath.begin(), mapPath.end());
	LPCWSTR windowsMapPath = *mapPath;
	mapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, this->mapSize, windowsMapPath);

	memPointer = static_cast<char*>(MapViewOfFile(memFile, FILE_MAP_ALL_ACCESS, 0, 0, this->memSize));
	mapPointer = static_cast<char*>(MapViewOfFile(mapFile, FILE_MAP_ALL_ACCESS, 0, 0, this->mapSize));
#elif PLATFORM_LINUX
    // Create the memory for the sensors and their mappings
    memFile = shm_open(memPath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
    mapFile = shm_open(mapPath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
    ftruncate(memFile, this->memSize);
    ftruncate(mapFile, this->mapSize);
    memPointer = static_cast<char*>(mmap(nullptr, this->memSize, PROT_READ | PROT_WRITE,
                                         MAP_SHARED, memFile, 0));
    mapPointer = static_cast<char*>(mmap(nullptr, this->mapSize, PROT_READ | PROT_WRITE,
                                         MAP_SHARED, mapFile, 0));
#endif
    *memPointer = '\0';
    *mapPointer = '\0';

    mapCurrentLength = 0;
    memLastIndex = 0;
}

void HolodeckSharedMemory::clear() {
    //TODO
}

bool HolodeckSharedMemory::containsKey(const FString &agentName, const FString &name) {
    return subscribedItems.count(agentName + "/" + name) > 0;
}

FString HolodeckSharedMemory::get(const FString &agentName, const FString &name) {
    SubscriberInfo info = subscribedItems.at(agentName + "/" + name);
    //std::string result(memPointer + info.getStartPos(), memPointer + info.getEndPos());
	return FString::FromHexBlob((uint8*)(memPointer + info.getStartPos()), info.getLength());
}

void HolodeckSharedMemory::subscribe(const FString &agentName, const FString &name, int size) {
    // Update
    updateSubscribedMap();

    FString key = agentName + "/" + name;

    // Check if it already exists in the map
    if (subscribedItems.count(key) > 0) {
        // If the size is wrong...
        SubscriberInfo info = subscribedItems[key];
        if (info.getLength() != size)
            throw std::invalid_argument("Size for sensor is not the same!");

        // Otherwise everything is fine, just return
        return;
    }

    // Update the data
    SubscriberInfo info(agentName, name, memLastIndex, memLastIndex + size, size);
    memLastIndex += size;

    // Write the information to the map file
    //std::stringstream ss;
	FString ss = info.getAgentName() + " " + info.getName() + " " + info.getStartPos() + " ";
	ss += info.getEndPos() + " " + info.getLength() + '\n';
    //ss << info.getAgentName() << " " << info.getName() << " " << info.getStartPos() << " ";
    //ss << info.getEndPos() << " " << info.getLength() << '\n';
    for (char c : ss) {
        mapPointer[mapCurrentLength] = c;
        mapCurrentLength++;
    }
    mapPointer[mapCurrentLength] = '\0';

    // Add it to the map
    subscribedItems.insert(std::pair<FString, SubscriberInfo>(info.getAgentName() + "/" +info.getName(), info));
}

void HolodeckSharedMemory::set(const FString &agentName, const FString &name, char *data) {
    FString key = agentName + "/" + name;
    SubscriberInfo info = subscribedItems[key];
	//if (length == 0)
	memcpy(memPointer + info.getStartPos(), data, strlen(data));
	//else
	//	memcpy(memPointer + info.getStartPos(), data, length);
}

int HolodeckSharedMemory::getLength() {
    int length = mapCurrentLength;
    for (int i = mapCurrentLength; i < mapSize; i++) {
        if (*(mapPointer + i) == '\0') {
            length = i;
            break;
        }
    }
    return length;
}

void HolodeckSharedMemory::updateSubscribedMap() {
    // if the length is the same, then return...
    int newLength = getLength();
    if (newLength == mapCurrentLength) return;
    mapCurrentLength = newLength;

    // Otherwise, read the new sensor data
    std::vector<FString> tokens;
    FString ss("");
    for (int i = mapCurrentLength; i < newLength; i++) {
        if (*(mapPointer + i) != ' ' && *(mapPointer + i) != '\n')
            ss += *(mapPointer + i);
        else {
            tokens.push_back(ss);
			ss.Empty();
        }
    }

    // Turn each token into data, and add it to the map
    SubscriberInfo info;
    for (int i = 0; i + 4 < tokens.size(); i+=5) {
        info.setData(tokens[i], tokens[i+1], FCString::Atoi(*tokens[i+2]), FCString::Atoi(*tokens[i+3]), FCString::Atoi(*tokens[i+4]));
        if (info.getEndPos() > memLastIndex) memLastIndex = info.getEndPos();
        subscribedItems.insert(std::pair<FString, SubscriberInfo>(info.getAgentName() + "/" +info.getName(), info));
    }

}

FString HolodeckSharedMemory::getData() const {
    FString s(memPointer, memPointer + memLastIndex);
    //std::cout << *memPointer << memPointer[1] << memPointer[2];
    return s;
}

FString HolodeckSharedMemory::getMappings() const {
    //std::stringstream ss;
    //ss << mapPointer;
    return FString(mapPointer);
}
