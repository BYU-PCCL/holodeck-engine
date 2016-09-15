// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "EasyFileManager.h"

EasyFileManager::EasyFileManager()
{
}

EasyFileManager::~EasyFileManager()
{
}

void EasyFileManager::SaveToFile(FString Data, FString FileName, FString Path, bool bOverwrite)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectoryTree(*Path))
	{
		FString AbsoluteFilePath = Path + FileName;
		if (bOverwrite || !PlatformFile.FileExists(*AbsoluteFilePath))
		{
			FFileHelper::SaveStringToFile(Data, *AbsoluteFilePath);
		}
	}
}
