// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/Texture2D.h"
#include "DDSLoader.h"

#include "TextureFromDiskFunctionLibrary.generated.h"

// BP Library for loading image files from disk at runtime.
//
// Written by n00854180t

UCLASS()
class TEXTUREFROMDISK_API UTextureFromDiskFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/** Creates a UTexture2D* from file path.
	* Read image and create Texture2D
	* @param FilePath		path to file to create sound wave from
	*/
	UFUNCTION(BlueprintCallable, Category = "TextureFromDisk")
	static class UTexture2D* GetTexture2DFromFile(const FString& FilePath);

private:
	// Thanks to @Ehamloptiran for the base code for loading a texture: 
	// https://docs.google.com/file/d/0BzqpBc4MrAMXLXN1ZGNzUEJZc1k/edit
};

