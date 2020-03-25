// Created by joshgreaves on 5/9/17.

#include "Holodeck.h"
#include "HolodeckServer.h"

#include <utility>
#include <ostream>

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
    if (bIsRunning) {
        UE_LOG(LogHolodeck, Warning, TEXT("HolodeckServer is already running! Bringing it down and up"));
        Kill();
    }

    if (!FParse::Value(FCommandLine::Get(), TEXT("HolodeckUUID="), UUID))
        UUID = "";
    UE_LOG(LogHolodeck, Log, TEXT("UUID: %s"), *UUID);

    if (!FParse::Bool(FCommandLine::Get(), TEXT("LogCalls="), bLogCalls))
        bLogCalls = false;

#if PLATFORM_WINDOWS
    auto LoadingSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, false, *(LOADING_SEMAPHORE_PATH + UUID));
    ReleaseSemaphore(LoadingSemaphore, 1, NULL);
    this->LockingSemaphore1 = CreateSemaphore(NULL, 1, 1, *(SEMAPHORE_PATH1 + UUID));
    this->LockingSemaphore2 = CreateSemaphore(NULL, 0, 1, *(SEMAPHORE_PATH2 + UUID));
#elif PLATFORM_LINUX
    LOG_CALL(sem_open(TCHAR_TO_ANSI(*(LOADING_SEMAPHORE_PATH + UUID)), O_CREAT, 0777, 0);)
    LOG_CALL(auto LoadingSemaphore = sem_open(TCHAR_TO_ANSI(*(LOADING_SEMAPHORE_PATH + UUID)), O_CREAT, 0777, 0);)
    if (LoadingSemaphore == SEM_FAILED) {
        LogSystemError("Unable to open loading semaphore");
    }

    LOG_CALL(int status = sem_post(LoadingSemaphore);)
    if (status == -1) {
        LogSystemError("Unable to update loading semaphore");
    }

    LOG_CALL(LockingSemaphore1 = sem_open(TCHAR_TO_ANSI(*(SEMAPHORE_PATH1 + UUID)), O_CREAT, 0777, 1);)
    if (LockingSemaphore1 == SEM_FAILED) {
        LogSystemError("Unable to open server semaphore");
    }

    LOG_CALL(LockingSemaphore2 = sem_open(TCHAR_TO_ANSI(*(SEMAPHORE_PATH2 + UUID)), O_CREAT, 0777, 0);)
    if (LockingSemaphore2 == SEM_FAILED) {
        LogSystemError("Unable to open client semaphore");
    }

    LOG_CALL(status = sem_unlink(LOADING_SEMAPHORE_PATH);)
    if (status == -1) {
        LogSystemError("Unable to close loading semaphore");
    }
#endif

    bIsRunning = true;
    UE_LOG(LogHolodeck, Log, TEXT("HolodeckServer started successfully"));
}

void UHolodeckServer::Kill() {
    UE_LOG(LogHolodeck, Log, TEXT("Killing HolodeckServer"));
    if (!bIsRunning) return;

    LOG_CALL(Memory.clear();)

#if PLATFORM_WINDOWS
    CloseHandle(this->LockingSemaphore1);
    CloseHandle(this->LockingSemaphore2);
#elif PLATFORM_LINUX
    LOG_CALL(int status = sem_unlink(SEMAPHORE_PATH1);)
    if (status == -1) {
        LogSystemError("Unable to close server semaphore");
    }

    LOG_CALL(status = sem_unlink(SEMAPHORE_PATH2);)
    if (status == -1) {
        LogSystemError("Unable to close client semaphore");
    }
#endif

    if (bLogCalls) {
        ExportFunctionCallLog();
    }

    bIsRunning = false;
    UE_LOG(LogHolodeck, Log, TEXT("HolodeckServer successfully shut down"));
}

void* UHolodeckServer::Malloc(const std::string& Key, unsigned int BufferSize) {
    // If this key doesn't already exist, or the buffer size has changed, allocate the memory.
    if (!Memory.count(Key) || Memory[Key]->Size() != BufferSize) {
        UE_LOG(LogHolodeck, Log, TEXT("Mallocing %u bytes for key %s"), BufferSize, UTF8_TO_TCHAR(Key.c_str()));
        Memory[Key] = std::unique_ptr<HolodeckSharedMemory>(new HolodeckSharedMemory(Key, BufferSize, TCHAR_TO_UTF8(*UUID)));
    }
    return Memory[Key]->GetPtr();
}

void UHolodeckServer::Acquire() {
    UE_LOG(LogHolodeck, VeryVerbose, TEXT("HolodeckServer Acquiring"));
#if PLATFORM_WINDOWS
    WaitForSingleObject(this->LockingSemaphore1, INFINITE);
#elif PLATFORM_LINUX

    LOG_CALL(int status = sem_wait(LockingSemaphore1);)
    if (status == -1) {
        LogSystemError("Unable to wait for server semaphore");
    }
#endif
}

void UHolodeckServer::Release() {
    UE_LOG(LogHolodeck, VeryVerbose, TEXT("HolodeckServer Releasing"));
#if PLATFORM_WINDOWS
    ReleaseSemaphore(this->LockingSemaphore2, 1, NULL);
#elif PLATFORM_LINUX

    LOG_CALL(int status = sem_post(LockingSemaphore2);)
    if (status == -1) {
        LogSystemError("Unable to update loading semaphore");
    }
#endif
}

bool UHolodeckServer::IsRunning() const {
    return bIsRunning;
}

void UHolodeckServer::LogSystemError(const std::string& errorMessage) {
    std::string errorMsg = errorMessage + " - Error code: " + std::to_string(errno) + " - " + std::string(strerror(errno));
    UE_LOG(LogHolodeck, Fatal, TEXT("%s"), ANSI_TO_TCHAR(errorMessage.c_str()));
}

void UHolodeckServer::LogCallDebug(std::string functionName) {
    FunctionCallInfo callInfo = {
            std::move(functionName),
            .timestamp =  std::chrono::system_clock::now().time_since_epoch().count(),
    };
    FunctionCalls.push_back(callInfo);
    // TODO: Find out how to avoid flushing on every log call
    ExportFunctionCallLog();
}

void UHolodeckServer::ExportFunctionCallLog() {
    UE_LOG(LogHolodeck, Log, TEXT("Exporting function call log"))
    std::ofstream callLogFileOut(FUNCTION_CALL_LOG_FILE_NAME, std::ofstream::out | std::ofstream::app);
    std::string stdStringUuid = TCHAR_TO_UTF8(*UUID);

    for (const auto &functionCall : FunctionCalls) {
        callLogFileOut << stdStringUuid << "," << functionCall.timestamp << "," << functionCall.functionName
                       << std::endl;
    }

    callLogFileOut.close();
    FunctionCalls.clear();
}
