// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetProcGen.h"

#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/UnrealMathUtility.h"

bool UPlanetProcGen::ShouldPixelContainPlanet(FIntPoint pixel)
{
	/*
	Procedurally generates the position of a planet inside this bucket if we determine that one should exist.

	const float pixelMagnitude = pixel.Size();

	const int moduloOffset = FGenericPlatformMath::Min((randomSeed >> 21), 100);
	const int modulo = FMath::FloorToInt(pixelMagnitude) + moduloOffset;

	return pixelMagnitude == 0 || (randomSeed << 7) % modulo != 0;
	*/

	return true;
}

void UPlanetProcGen::ProcGenAroundPlayer(FVector2D position)
{
	const int SQUARE_WIDTH = 10;

	// Convert the float vector to a pixel
	FIntPoint positionAsPixel{ FMath::FloorToInt(position.X), FMath::FloorToInt(position.Y) };

	int xMin = positionAsPixel.X - SQUARE_WIDTH;
	int xMax = positionAsPixel.X + SQUARE_WIDTH;
	int yMin = positionAsPixel.Y - SQUARE_WIDTH;
	int yMax = positionAsPixel.Y + SQUARE_WIDTH;

	for ( int row_index = xMin; row_index < xMax; ++row_index )
	{
		for ( int column_index = yMin; column_index < yMax; ++column_index )
		{
			FIntPoint gridPoint{ row_index, column_index };

			if (blankPixels.Find(gridPoint) || planetData.Find(gridPoint))
			{
				continue;
			}
			else if (ShouldPixelContainPlanet( gridPoint ))
			{
				APlanet * newPlanet = GetWorld()->SpawnActor<APlanet>(defaultPlanetClass);
				FVector location{ static_cast<float>(row_index) + 0.5f, static_cast<float>(column_index) + 0.5f, 0.0f };
				newPlanet->SetActorLocation(location);
				planetData.Add(gridPoint, newPlanet);
			}
			else
			{
				blankPixels.Add(gridPoint);
			}
		}
	}
}