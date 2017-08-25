// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <map>
#include <memory>
#include "HolodeckSharedmemory.h"

#if PLATFORM_WINDOWS
#define SEMAPHORE_PATH1 "Global\\HOLODECK_SEMAPHORE_1"
#define SEMAPHORE_PATH2 "Global\\HOLODECK_SEMAPHORE_2"
#include "AllowWindowsPlatformTypes.h"
#include "Windows.h"
#include "HideWindowsPlatformTypes.h"
#elif PLATFORM_LINUX
#define SEMAPHORE_PATH1 "/HOLODECK_SEMAPHORE_1"
#define SEMAPHORE_PATH2 "/HOLODECK_SEMAPHORE_2"
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#endif

#include "HolodeckServer.generated.h"

/**
* This class resides in Holodeck, and handles the
* passing of messages through shared memory.
* A new shared memory block is created for each sensor and for each action space.
* There should only be one UHolodeckServer, and it should be instantiated by HolodeckGameInstance.
*/
UCLASS()
class HOLODECK_API UHolodeckServer : public UObject
{
	GENERATED_BODY()

public:

	void start();

	void kill();

	/**
	* Subscribes a sensor.
	* If a sensor with that name already exists in the memory, and the size is the same
	* then it is allocated that space, otherwise an invalid argument exception is thrown.
	* @param agentName the name of the agent this sensor belongs to
	* @param sensorKey the name of the sensor
	* @param size the size of the data to allocate
	*/
	float* subscribeSensor(const std::string& agent_name, const std::string& sensor_key, int size);

	void acquire();

	void release();

	~UHolodeckServer();

	UHolodeckServer();

	bool isRunning() const;

private:

	std::string makeKey(const std::string& agent_name, const std::string& sensor_name);

#if PLATFORM_WINDOWS
	HANDLE lockingSemaphore1;
	HANDLE lockingSemaphore2;
#elif PLATFORM_LINUX
	sem_t* lockingSemaphore1;
	sem_t* lockingSemaphore2;
#endif

	std::map<std::string, std::unique_ptr<HolodeckSharedMemory>> sensors;

	UPROPERTY()
	bool bIsRunning;
};


