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

	sensors = new HolodeckSharedMemory(SENSOR_PATH, SENSOR_MEM_SIZE, SENSOR_MAP_SIZE);
	commands = new HolodeckSharedMemory(COMMAND_PATH, COMMAND_MEM_SIZE, COMMAND_MEM_SIZE);
	settings = new HolodeckSharedMemory(SETTINGS_PATH, SETTINGS_MEM_SIZE, SETTINGS_MAP_SIZE);

#if PLATFORM_WINDOWS
	this->mutex1 = CreateMutex(NULL, true, TEXT(MUTEX_PATH1));
	this->mutex2 = CreateMutex(NULL, true, TEXT(MUTEX_PATH2));
#elif PLATFORM_LINUX
	// TODO change to MUTEX
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
	delete sensors;
	delete commands;
	delete settings;
	sensors = nullptr;
	commands = nullptr;
	settings = nullptr;
	CloseHandle(this->mutex1);
	CloseHandle(this->mutex2);
	bIsRunning = false;
}

UHolodeckServer::~UHolodeckServer() {
	kill();
}

//UHolodeckServer* UHolodeckServer::getInstance() {
//	if (instance == nullptr)
//		instance = NewObject<UHolodeckServer>();
//	return instance;
//}

void UHolodeckServer::subscribeSensor(const FString &agentName,
	const FString &sensorKey,
	int size) {
	sensors->subscribe(agentName, sensorKey, size);
}

void UHolodeckServer::setSensor(const FString &agentName, const FString &sensorKey, char *data) {
	sensors->set(agentName, sensorKey, data);
}

FString UHolodeckServer::getSensor(const FString &agentName, const FString &sensorKey) {
	return sensors->get(agentName, sensorKey);
}

FString UHolodeckServer::getSensorsData() const {
	return sensors->getData();
}

FString UHolodeckServer::getSensorsMapping() const {
	return sensors->getMappings();
}

void UHolodeckServer::subscribeCommand(const FString &agentName,
	const FString &commandKey,
	int size) {
	commands->subscribe(agentName, commandKey, size);
}

void UHolodeckServer::setCommand(const FString &agentName, const FString &commandKey, char *data) {
	commands->set(agentName, commandKey, data);
}

FString UHolodeckServer::getCommand(const FString &agentName, const FString &commandKey) {
	return commands->get(agentName, commandKey);
}

FString UHolodeckServer::getCommandsData() const {
	return commands->getData();
}

FString UHolodeckServer::getCommandsMapping() const {
	return commands->getMappings();
}

void UHolodeckServer::subscribeSetting(const FString &agentName,
	const FString &settingKey,
	int size) {
	settings->subscribe(agentName, settingKey, size);
}

void UHolodeckServer::setSetting(const FString &agentName, const FString &settingKey, char *data) {
	settings->set(agentName, settingKey, data);
}

FString UHolodeckServer::getSetting(const FString &agentName, const FString &settingKey) {
	return settings->get(agentName, settingKey);
}

FString UHolodeckServer::getSettingsData() const {
	return settings->getData();
}

FString UHolodeckServer::getSettingsMapping() const {
	return settings->getMappings();
}

void UHolodeckServer::acquire() {
#if PLATFORM_WINDOWS
	UE_LOG(LogHolodeck, Log, TEXT("Acquiring mutex..."));
	WaitForSingleObject(this->mutex1, INFINITE);
	UE_LOG(LogHolodeck, Log, TEXT("Acquired!"));
#elif PLATFROM_LINUX
	sem_wait(lockingSemaphore1);
#endif
}

void UHolodeckServer::release() {
	// TODO: Ensure that the value is 0 before releasing it
#if PLATFORM_WINDOWS
	UE_LOG(LogHolodeck, Log, TEXT("Releasing mutex..."));
	ReleaseMutex(this->mutex2);
	UE_LOG(LogHolodeck, Log, TEXT("Released!"));
#elif PLATFORM_LINX
	sem_post(lockingSemaphore2);
#endif
}

bool UHolodeckServer::isRunning() const {
	return bIsRunning;
}
