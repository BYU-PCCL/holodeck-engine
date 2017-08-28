//
// Created by josh on 5/9/17.
//

#include "Holodeck.h"
#include <cstring>
#include <vector>
#include "HolodeckServer.h"

UHolodeckServer::UHolodeckServer() {
	// Warning -- This class gets initialized a few times by Unreal because it is a UObject. 
	// DO NOT rely on singleton-qualities in the constructor, only in the Start() function
	bIsRunning = false;
}

void UHolodeckServer::start() {
	if (bIsRunning) kill();

#if PLATFORM_WINDOWS
	this->lockingSemaphore1 = CreateSemaphore(NULL, 1, 1, TEXT(SEMAPHORE_PATH1));
	this->lockingSemaphore2 = CreateSemaphore(NULL, 0, 1, TEXT(SEMAPHORE_PATH2));
#elif PLATFORM_LINUX
	// Create the semaphores - Currently destroys existing semaphores
	sem_unlink(SEM_PATH1);
	sem_unlink(SEM_PATH2);
	lockingSemaphore1 = sem_open(SEM_PATH1, O_CREAT, 0777, 1);
	lockingSemaphore2 = sem_open(SEM_PATH2, O_CREAT, 0777, 0);
#endif

	bIsRunning = true;
}

void UHolodeckServer::kill() {
	if (!bIsRunning) return;
	sensors.clear();
	CloseHandle(this->lockingSemaphore1);
	CloseHandle(this->lockingSemaphore2);
	bIsRunning = false;
}

UHolodeckServer::~UHolodeckServer() {
	kill();
}

float* UHolodeckServer::subscribeSensor(const std::string& agent_name, const std::string& sensor_key, int size) {
	std::string key = makeKey(agent_name, sensor_key);
	sensors[key] = std::unique_ptr<HolodeckSharedMemory>(new HolodeckSharedMemory(key, size));
	return sensors[key]->get();
}

void UHolodeckServer::acquire() {
#if PLATFORM_WINDOWS
	UE_LOG(LogHolodeck, Log, TEXT("Acquiring semaphore..."));
	WaitForSingleObject(this->lockingSemaphore1, INFINITE);
	UE_LOG(LogHolodeck, Log, TEXT("Acquired!"));
#elif PLATFROM_LINUX
	sem_wait(lockingSemaphore1);
#endif
}

void UHolodeckServer::release() {
	// TODO: Ensure that the value is 0 before releasing it
#if PLATFORM_WINDOWS
	ReleaseSemaphore(this->lockingSemaphore2, 1, NULL);
#elif PLATFORM_LINX
	sem_post(lockingSemaphore2);
#endif
}

bool UHolodeckServer::isRunning() const {
	return bIsRunning;
}

std::string UHolodeckServer::makeKey(const std::string& agent_name, const std::string& sensor_name) {
	return agent_name + "/" + sensor_name;
}
