// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class HOLODECK_API EasyFileManager
{
public:
	EasyFileManager();
	~EasyFileManager();

	void SaveToFile(FString Data, FString FileName, FString Path = "C:\\Users\\robert.pottorff\\Desktop\\", bool bOverwrite = true);
};
