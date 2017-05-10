// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <map>
#include "HolodeckSharedmemory.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_LINUX
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#endif

#define SENSOR_PATH "SENSORS"
#define SETTINGS_PATH "SETTINGS"
#define COMMAND_PATH "COMMANDS"

#define SENSOR_MEM_SIZE 33177600 // Enough space for 2 RGBA images at 1920 x 1080, around 33mb
#define COMMAND_MEM_SIZE 10000
#define SETTINGS_MEM_SIZE 10000
#define SENSOR_MAP_SIZE 10000
#define COMMAND_MAP_SIZE 10000
#define SETTINGS_MAP_SIZE 10000

#define SEM_PATH1 "/HOLODECK_SEMAPHORE_1"
#define SEM_PATH2 "/HOLODECK_SEMAPHORE_2"

/**
* This class resides in Holodeck, and handles the
* passing of messages through shared memory.
* It contains six blocks of shared memory, 2 mapping blocks and 3 for data.
* The mapping blocks are for explaining what lies in the corresponding data memory.
* The three types are sensors, commands, and settings (settings needs no mapping block, it is text).
* HolodeckServer follows the Singleton design pattern, and can be obtained with the getInstance function.
* Should be instantiated by GameInstance
*/
class HOLODECK_API HolodeckServer
{
public:
	/**
     * Gets a reference to the HolodeckServer
     * Following the singleton design pattern, this allows only one holodeck server object.
     * @return a reference to the HolodeckServer object.
     */
	static HolodeckServer& getInstance();

	/**
	* Subscribes a sensor to a part of the memory
	* If a sensor with that name already exists in the memory, and the size is the same
	* then it is allocated that space, otherwise an invalid argument exception is thrown.
	* @param agentName the name of the agent this sensor belongs to
	* @param sensorKey the name of the sensor
	* @param size the size of the data to allocate
	*/
	void subscribeSensor(const FString &agentName, const FString &sensorKey, int size);

	void setSensor(const FString &agentName, const FString &sensorKey, char *data);

	FString getSensor(const FString &agentName, const FString &sensorKey);

	FString getSensorsData() const;
	FString getSensorsMapping() const;

	void subscribeCommand(const FString &agentName, const FString& commandKey, int size);

	void setCommand(const FString& agentName, const FString& commandKey, char* data);

	FString getCommand(const FString& agentName, const FString& commandKey);

	FString getCommandsData() const;
	FString getCommandsMapping() const;

	void subscribeSetting(const FString &agentName, const FString& settingKey, int size);

	void setSetting(const FString& agentName, const FString& settingKey, char* data);

	FString getSetting(const FString& agentName, const FString& settingKey);

	FString getSettingsData() const;
	FString getSettingsMapping() const;

	void acquire();

	void release();

	~HolodeckServer();


protected:
	HolodeckServer();

private:

#if PLATFORM_WINDOWS
	HANDLE lockingSemaphore1;
	HANDLE lockingSemaphore2;
#elif PLATFORM_LINUX
	sem_t* lockingSemaphore1;
	sem_t* lockingSemaphore2;
#endif
	HolodeckSharedMemory* sensors;
	HolodeckSharedMemory* commands;
	HolodeckSharedMemory* settings;

	void updateSensors();

	/*
	 * Locks a CriticalSection object that is used to prevent simultaneous read/write on a socket by two threads
	 */
	void LockSocket();

	/*
	 * Unlocks a CriticalSection object that is used to prevent simultaneous read/write on a socket by two threads
	 */
	void UnlockSocket();
};


