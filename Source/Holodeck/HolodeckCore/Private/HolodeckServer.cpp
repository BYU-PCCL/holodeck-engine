// Created by joshgreaves on 5/9/17.

#include "Holodeck.h"
#include "HolodeckServer.h"

UHolodeckServer::UHolodeckServer() {
	// Warning -- This class gets initialized a few times by Unreal because it is a UObject. 
	// DO NOT rely on singleton-qualities in the constructor, only in the Start() function
	bIsRunning = false;
}

UHolodeckServer::~UHolodeckServer() {
	Kill();
}

void UHolodeckServer::Start() {
	if (bIsRunning) Kill();

	#if PLATFORM_WINDOWS
	this->LockingSemaphore1 = CreateSemaphore(NULL, 1, 1, TEXT(SEMAPHORE_PATH1));
	this->LockingSemaphore2 = CreateSemaphore(NULL, 0, 1, TEXT(SEMAPHORE_PATH2));
	#elif PLATFORM_LINUX
	sem_unlink(SEMAPHORE_PATH1);
	sem_unlink(SEMAPHORE_PATH2);
	LockingSemaphore1 = sem_open(SEMAPHORE_PATH1, O_CREAT, 0777, 1);
	LockingSemaphore2 = sem_open(SEMAPHORE_PATH2, O_CREAT, 0777, 0);
	#endif

	bIsRunning = true;
}

void UHolodeckServer::Kill() {
	if (!bIsRunning) return;

	Sensors.clear();
	ActionSpaces.clear();
	Settings.clear();

	#if PLATFORM_WINDOWS
	CloseHandle(this->LockingSemaphore1);
	CloseHandle(this->LockingSemaphore2);
	#elif PLATFORM_LINUX
	sem_unlink(SEMAPHORE_PATH1);
	sem_unlink(SEMAPHORE_PATH2);
	#endif

	bIsRunning = false;
}

void* UHolodeckServer::SubscribeSensor(const std::string& AgentName, const std::string& SensorKey, int BufferSize) {
	std::string Key = MakeKey(AgentName, SensorKey);
	Sensors[Key] = std::unique_ptr<HolodeckSharedMemory>(new HolodeckSharedMemory(Key, BufferSize));
	return Sensors[Key]->GetPtr();
}

void* UHolodeckServer::SubscribeActionSpace(const std::string& AgentName, int BufferSize) {
	ActionSpaces[AgentName] = std::unique_ptr<HolodeckSharedMemory>(new HolodeckSharedMemory(AgentName, BufferSize));
	return ActionSpaces[AgentName]->GetPtr();
}


void* UHolodeckServer::SubscribeSetting(const std::string& SettingName, int BufferSize) {
	Settings[SettingName] = std::unique_ptr<HolodeckSharedMemory>(new HolodeckSharedMemory(SettingName, BufferSize));
	return Settings[SettingName]->GetPtr();
}

void UHolodeckServer::Acquire() {
	#if PLATFORM_WINDOWS
	WaitForSingleObject(this->LockingSemaphore1, INFINITE);
	#elif PLATFROM_LINUX
	sem_wait(LockingSemaphore1);
	#endif
}

void UHolodeckServer::Release() {
	#if PLATFORM_WINDOWS
	ReleaseSemaphore(this->LockingSemaphore2, 1, NULL);
	#elif PLATFORM_LINX
	sem_post(LockingSemaphore2);
	#endif
}

bool UHolodeckServer::IsRunning() const {
	return bIsRunning;
}

std::string UHolodeckServer::MakeKey(const std::string& AgentName, const std::string& SensorName) const {
	return AgentName + "/" + SensorName;
}
