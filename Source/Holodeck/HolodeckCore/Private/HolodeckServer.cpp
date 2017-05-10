//
// Created by josh on 5/9/17.
//

#include "Holodeck.h"
#include <cstring>
#include <vector>
#include "HolodeckServer.h"

HolodeckServer::HolodeckServer() {
	sensors = new HolodeckSharedMemory(SENSOR_PATH, SENSOR_MEM_SIZE, SENSOR_MAP_SIZE);
	commands = new HolodeckSharedMemory(COMMAND_PATH, COMMAND_MEM_SIZE, COMMAND_MEM_SIZE);
	settings = new HolodeckSharedMemory(SETTINGS_PATH, SETTINGS_MEM_SIZE, SETTINGS_MAP_SIZE);

#if PLATFORM_WINDOWS
	lockingSemaphore1 = CreateSemaphore(NULL, 1, 1, TEXT(SEM_PATH1));
	lockingSemaphore2 = CreateSemaphore(NULL, 1, 0, TEXT(SEM_PATH2));
#elif PLATFORM_LINUX
	// Create the semaphores - Currently destroys existing semaphores
	sem_unlink(SEM_PATH1);
	sem_unlink(SEM_PATH2);
	lockingSemaphore1 = sem_open(SEM_PATH1, O_CREAT, 0777, 1);
	lockingSemaphore2 = sem_open(SEM_PATH2, O_CREAT, 0777, 0);
#endif
}

HolodeckServer::~HolodeckServer() {
	delete sensors;
	delete commands;
	delete settings;
}

HolodeckServer &HolodeckServer::getInstance() {
	static HolodeckServer instance;
	return instance;
}

void HolodeckServer::subscribeSensor(const FString &agentName,
	const FString &sensorKey,
	int size) {
	sensors->subscribe(agentName, sensorKey, size);
}

void HolodeckServer::setSensor(const FString &agentName, const FString &sensorKey, char *data) {
	sensors->set(agentName, sensorKey, data);
}

FString HolodeckServer::getSensor(const FString &agentName, const FString &sensorKey) {
	return sensors->get(agentName, sensorKey);
}

FString HolodeckServer::getSensorsData() const {
	return sensors->getData();
}

FString HolodeckServer::getSensorsMapping() const {
	return sensors->getMappings();
}

void HolodeckServer::subscribeCommand(const FString &agentName,
	const FString &commandKey,
	int size) {
	commands->subscribe(agentName, commandKey, size);
}

void HolodeckServer::setCommand(const FString &agentName, const FString &commandKey, char *data) {
	commands->set(agentName, commandKey, data);
}

FString HolodeckServer::getCommand(const FString &agentName, const FString &commandKey) {
	return commands->get(agentName, commandKey);
}

FString HolodeckServer::getCommandsData() const {
	return commands->getData();
}

FString HolodeckServer::getCommandsMapping() const {
	return commands->getMappings();
}

void HolodeckServer::subscribeSetting(const FString &agentName,
	const FString &settingKey,
	int size) {
	settings->subscribe(agentName, settingKey, size);
}

void HolodeckServer::setSetting(const FString &agentName, const FString &settingKey, char *data) {
	settings->set(agentName, settingKey, data);
}

FString HolodeckServer::getSetting(const FString &agentName, const FString &settingKey) {
	return settings->get(agentName, settingKey);
}

FString HolodeckServer::getSettingsData() const {
	return settings->getData();
}

FString HolodeckServer::getSettingsMapping() const {
	return settings->getMappings();
}

void HolodeckServer::acquire() {
#if PLATFORM_WINDOWS
	WaitForSingleObject(lockingSemaphore1, INFINITE);
#elif PLATFROM_LINUX
	sem_wait(lockingSemaphore1);
#endif
}

void HolodeckServer::release() {
	// TODO: Ensure that the value is 0 before releasing it
#if PLATFORM_WINDOWS
	ReleaseSemaphore(lockingSemaphore2, 1, NULL);
#elif PLATFORM_LINX
	sem_post(lockingSemaphore2);
#endif
}
