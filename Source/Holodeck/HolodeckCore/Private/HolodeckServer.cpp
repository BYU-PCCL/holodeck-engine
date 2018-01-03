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
	UE_LOG(LogHolodeck, Log, TEXT("Initializing HolodeckServer"));
	if (bIsRunning) Kill();

	if (!FParse::Value(FCommandLine::Get(), TEXT("HolodeckUUID="), UUID))
		UUID = "";
	UE_LOG(LogHolodeck, Log, TEXT("UUID: %s"), *UUID);

	#if PLATFORM_WINDOWS
	this->LockingSemaphore1 = CreateSemaphore(NULL, 1, 1, *(SEMAPHORE_PATH1 + UUID));
	this->LockingSemaphore2 = CreateSemaphore(NULL, 0, 1, *(SEMAPHORE_PATH2 + UUID));
	#elif PLATFORM_LINUX
	sem_unlink(SEMAPHORE_PATH1);
	sem_unlink(SEMAPHORE_PATH2);
	LockingSemaphore1 = sem_open(TCHAR_TO_ANSI(*(SEMAPHORE_PATH1 + UUID)), O_CREAT, 0777, 1);
	LockingSemaphore2 = sem_open(TCHAR_TO_ANSI(*(SEMAPHORE_PATH2 + UUID)), O_CREAT, 0777, 0);
	#endif

	bIsRunning = true;
}

void UHolodeckServer::Kill() {
	UE_LOG(LogHolodeck, Log, TEXT("Killing HolodeckServer"));
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
	UE_LOG(LogHolodeck, Log, TEXT("Subscribing sensor %s for agent %s"), UTF8_TO_TCHAR(SensorKey.c_str()), UTF8_TO_TCHAR(AgentName.c_str()));
	std::string Key = MakeKey(AgentName, SensorKey);
	Sensors[Key] = std::unique_ptr<HolodeckSharedMemory>(new HolodeckSharedMemory(Key, BufferSize, TCHAR_TO_UTF8(*UUID)));
	return Sensors[Key]->GetPtr();
}

void* UHolodeckServer::SubscribeActionSpace(const std::string& AgentName, int BufferSize) {
	UE_LOG(LogHolodeck, Log, TEXT("Subscribing action space for %s"), UTF8_TO_TCHAR(AgentName.c_str()));
	ActionSpaces[AgentName] = std::unique_ptr<HolodeckSharedMemory>(new HolodeckSharedMemory(AgentName, BufferSize, TCHAR_TO_UTF8(*UUID)));
	return ActionSpaces[AgentName]->GetPtr();
}

void* UHolodeckServer::SubscribeSetting(const std::string& SettingName, int BufferSize) {
	Settings[SettingName] = std::unique_ptr<HolodeckSharedMemory>(new HolodeckSharedMemory(SettingName, BufferSize, TCHAR_TO_UTF8(*UUID)));
	return Settings[SettingName]->GetPtr();
}

void UHolodeckServer::Acquire() {
	#if PLATFORM_WINDOWS
	WaitForSingleObject(this->LockingSemaphore1, INFINITE);
	#elif PLATFORM_LINUX
	sem_wait(LockingSemaphore1);
	#endif
}

void UHolodeckServer::Release() {
	#if PLATFORM_WINDOWS
	ReleaseSemaphore(this->LockingSemaphore2, 1, NULL);
	#elif PLATFORM_LINUX
	sem_post(LockingSemaphore2);
	#endif
}

bool UHolodeckServer::IsRunning() const {
	return bIsRunning;
}

std::string UHolodeckServer::MakeKey(const std::string& AgentName, const std::string& SensorName) const {
	return AgentName + "_" + SensorName;
}
