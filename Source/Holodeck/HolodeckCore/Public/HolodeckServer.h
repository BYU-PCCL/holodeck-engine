// Created by joshgreaves on 5/9/17.

#pragma once

#include "Holodeck.h"

#include <map>
#include <memory>

#include "HolodeckSharedMemory.h"
#if PLATFORM_WINDOWS
#define LOADING_SEMAPHORE_PATH "Global\\HOLODECK_LOADING_SEM"
#define SEMAPHORE_PATH1 "Global\\HOLODECK_SEMAPHORE_SERVER"
#define SEMAPHORE_PATH2 "Global\\HOLODECK_SEMAPHORE_CLIENT"
#include "AllowWindowsPlatformTypes.h"
#include "Windows.h"
#include "HideWindowsPlatformTypes.h"
#elif PLATFORM_LINUX
#define LOADING_SEMAPHORE_PATH "/HOLODECK_LOADING_SEM"
#define SEMAPHORE_PATH1 "/HOLODECK_SEMAPHORE_SERVER"
#define SEMAPHORE_PATH2 "/HOLODECK_SEMAPHORE_CLIENT"
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#endif

#include "HolodeckServer.generated.h"

/**
* This class resides in Holodeck, and handles the passing of messages through
* shared memory. A new shared memory block is created for each sensor, action
* space, and setting.
* There should only be one UHolodeckServer, and it should be instantiated by
* HolodeckGameInstance.
*/
UCLASS()
class HOLODECK_API UHolodeckServer : public UObject {
	GENERATED_BODY()

public:

	/**
	* Default Constructor
	*/
	UHolodeckServer();

	/**
	* Default Destructor
	*/
	~UHolodeckServer();

	/**
	* Start
	* Starts the server.
	*/
	void Start();

	/**
	* Kill
	* Shuts the server down.
	*/
	void Kill();

	/**
	* SubscribeSensor
	* Subscribes a sensor. Creates a shared memory block for the sensor.
	* If a sensor already exists in this project with the same name and
	* same agent, the block is overwritten with the new sensor.
	* @param AgentName the name of the agent this sensor belongs to.
	* @param SensorKey the name of the sensor.
	* @param BufferSize the size to allocate in bytes.
	* @return a pointer to the start of the assigned memory.
	*/
	void* SubscribeSensor(const std::string& AgentName, const std::string& SensorKey, int BufferSize);

	/**
	* SubscribeActionSpace
	* Subscribes an action space. Creates a shared memory block for the
	* action space.
	* @param AgentName the agent to subscribe space for.
	* @param BufferSize the size of the buffer to allocate in bytes.
	* @return a poitner to the start of the assigned memory.
	*/
	void* SubscribeActionSpace(const std::string& AgentName, int BufferSize);

	/**
	* SubscribeSetting
	* Subscribes a setting. Creates a shared memory block for the setting.
	* @param SettingName the name of the setting.
	* @param BufferSize the size of the buffer to allocate in bytes.
	* @return a pointer to the start of the assigned memory.
	*/
	void* SubscribeSetting(const std::string& SettingName, int BufferSize);

	/**
	* Acquire
	* Acquires the mutex to allow the next tick to occur. Will block until
	* the client releases.
	*/
	void Acquire();

	/**
	* Release
	* Releases the mutex to allow the client to tick.
	*/
	void Release();

	/**
	* IsRunning
	* Gets whether the server is running.
	* @return true if the server is running.
	*/
	bool IsRunning() const;

private:

	/**
	* MakeKey
	* Makes the key for subscribing sensors.
	* @param AgentName the name of the agent.
	* @param SensorName the name of the sensor.
	* @return a key for this agent/sensor for the map.
	*/
	std::string MakeKey(const std::string& AgentName, const std::string& SensorName) const;

	std::map<std::string, std::unique_ptr<HolodeckSharedMemory>> Sensors;
	std::map<std::string, std::unique_ptr<HolodeckSharedMemory>> ActionSpaces;
	std::map<std::string, std::unique_ptr<HolodeckSharedMemory>> Settings;

	UPROPERTY()
		bool bIsRunning;

	#if PLATFORM_WINDOWS
	HANDLE LockingSemaphore1;
	HANDLE LockingSemaphore2;
	#elif PLATFORM_LINUX
	sem_t* LockingSemaphore1;
	sem_t* LockingSemaphore2;
	#endif

	FString UUID;
};
