//
// Created by joshgreaves on 5/9/17.
//

#include "Holodeck.h"
#include "HolodeckSharedMemory.h"

const char HOLODECK_BASE_PATH[] = "/HOLODECK_MEM";

#if PLATFORM_WINDOWS
// Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString()
{
	// Get the error message, if any.
	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0) {
		return "NO ERROR - NO COLLUSION"; // No error message has been recorded
	}

	UE_LOG(LogHolodeck, Error, TEXT("Error: %d"), errorMessageID);

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	// Free the buffer.
	LocalFree(messageBuffer);

	return message;
}
#endif

HolodeckSharedMemory::HolodeckSharedMemory(const std::string& Name, unsigned int BufferSize, const std::string& UUID) :
		MemPath(HOLODECK_BASE_PATH + UUID + "_" + Name), MemSize(BufferSize) {

	#if PLATFORM_WINDOWS

	std::wstring STemp = std::wstring(MemPath.begin(), MemPath.end());
	LPCWSTR WindowsMemPath = STemp.c_str();

	UE_LOG(LogHolodeck, Log, TEXT("HolodeckSharedMemory:: Creating file mapping of size %d with path %s..."), BufferSize, ANSI_TO_TCHAR(MemPath.c_str()));

	MemFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, this->MemSize, WindowsMemPath);

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckSharedMemory:: Unable to create MemFile! The path %s already exists. Hopefully it is the correct size..."), ANSI_TO_TCHAR(MemPath.c_str()));
	}

	if (!MemFile) {
		std::string msg = GetLastErrorAsString();
		UE_LOG(LogHolodeck, Fatal, TEXT("HolodeckSharedMemory:: Unable to create MemFile! %s"), ANSI_TO_TCHAR(msg.c_str()));
	}

	MemPointer = static_cast<void*>(MapViewOfFile(MemFile, FILE_MAP_ALL_ACCESS, 0, 0, 0));

	if (!MemPointer) {
		std::string msg = GetLastErrorAsString();
		UE_LOG(LogHolodeck, Fatal, TEXT("HolodeckSharedMemory:: Unable to create MemPointer! %s"), ANSI_TO_TCHAR(msg.c_str()));
	}

	#elif PLATFORM_LINUX

    MemFile = shm_open(MemPath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
    ftruncate(MemFile, this->MemSize);
    MemPointer = static_cast<void*>(mmap(nullptr, this->MemSize, PROT_READ | PROT_WRITE,
                                         MAP_SHARED, MemFile, 0));
	#endif
}

HolodeckSharedMemory::~HolodeckSharedMemory() {
	#if PLATFORM_WINDOWS
	// TODO: Correctly close these files
	#elif PLATFORM_LINUX
	// TODO: Correctly close these files
	close(MemFile);
	#endif
}
