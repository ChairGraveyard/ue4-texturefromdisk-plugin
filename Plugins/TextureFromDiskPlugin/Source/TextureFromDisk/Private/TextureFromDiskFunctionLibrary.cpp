// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#include "TextureFromDiskPrivatePCH.h"

//////////////////////////////////////////////////////////////////////////
// UOggBPFunctionLibrary

UTextureFromDiskFunctionLibrary::UTextureFromDiskFunctionLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	 
}

class UTexture2D* UTextureFromDiskFunctionLibrary::GetTexture2DFromFile(const FString& FilePath)
{
	UTexture2D* Texture = NULL;

	FString TexturePath = FilePath;//FPaths::GameContentDir( ) + TEXT( "../Data/" ) + TextureFilename;
	TArray<uint8> FileData;

	/* Load DDS texture */
	if( FFileHelper::LoadFileToArray( FileData, *TexturePath, 0 ) )
	{
		FDDSLoadHelper DDSLoadHelper( &FileData[ 0 ], FileData.Num( ) );
		if( DDSLoadHelper.IsValid2DTexture( ) )
		{
			int32 NumMips = DDSLoadHelper.ComputeMipMapCount( );
			EPixelFormat Format = DDSLoadHelper.ComputePixelFormat( );
			int32 BlockSize = 16;

			if( NumMips == 0 )
			{
				NumMips = 1;
			}

			if( Format == PF_DXT1 )
			{
				BlockSize = 8;
			}

			/* Create transient texture */
			Texture = UTexture2D::CreateTransient( DDSLoadHelper.DDSHeader->dwWidth, DDSLoadHelper.DDSHeader->dwHeight, Format );
			Texture->MipGenSettings = TMGS_LeaveExistingMips;
			Texture->PlatformData->NumSlices = 1;
			Texture->NeverStream = true;
			//Texture->Rename(  );

			/* Get pointer to actual data */
			uint8* DataPtr = (uint8*) DDSLoadHelper.GetDDSDataPointer( );

			uint32 CurrentWidth = DDSLoadHelper.DDSHeader->dwWidth;
			uint32 CurrentHeight = DDSLoadHelper.DDSHeader->dwHeight;

			/* Iterate through mips */
			for( int32 i = 0; i < NumMips; i++ )
			{
				/* Lock to 1x1 as smallest size */
				CurrentWidth = ( CurrentWidth < 1 ) ? 1 : CurrentWidth;
				CurrentHeight = ( CurrentHeight < 1 ) ? 1 : CurrentHeight;

				/* Get number of bytes to read */
				int32 NumBytes = CurrentWidth * CurrentHeight * 4;
				if( Format == PF_DXT1 || Format == PF_DXT3 || Format == PF_DXT5 )
				{
					/* Compressed formats */
					NumBytes = ( ( CurrentWidth + 3 ) / 4 ) * ( ( CurrentHeight + 3 ) / 4 ) * BlockSize;
				}

				/* Write to existing mip */
				if( i < Texture->PlatformData->Mips.Num( ) )
				{
					FTexture2DMipMap& Mip = Texture->PlatformData->Mips[ i ];

					void* Data = Mip.BulkData.Lock( LOCK_READ_WRITE );
					FMemory::Memcpy( Data, DataPtr, NumBytes );
					Mip.BulkData.Unlock( );
				}

				/* Add new mip */
				else
				{
					FTexture2DMipMap* Mip = new( Texture->PlatformData->Mips ) FTexture2DMipMap( );
					Mip->SizeX = CurrentWidth;
					Mip->SizeY = CurrentHeight;

					Mip->BulkData.Lock( LOCK_READ_WRITE );
					Mip->BulkData.Realloc( NumBytes );
					Mip->BulkData.Unlock( );

					void* Data = Mip->BulkData.Lock( LOCK_READ_WRITE );
					FMemory::Memcpy( Data, DataPtr, NumBytes );
					Mip->BulkData.Unlock( );
				}

				/* Set next mip level */
				CurrentWidth /= 2;
				CurrentHeight /= 2;

				DataPtr += NumBytes;
			}

			Texture->UpdateResource( );
		}
	}

	return Texture;
}
