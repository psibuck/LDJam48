// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetProcGen.h"

#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/UnrealMathUtility.h"

bool UPlanetProcGen::ShouldGenerateObjectInLocationWithCoefficients(FIntPoint pixel, int xCoefficient, int yCoefficient)
{
	// Procedurally determines whether this pixel should contain an object given the following coefficients

	const int GRID_STEP = GetGridStep();

	const int baseX = FMath::Abs(pixel.X / cSparsity);
	const int baseY = FMath::Abs(pixel.Y / cSparsity);

	const int baseXModulo = FMath::Max(FMath::Abs(GenerateSparsityModulo(baseX + (xCoefficient * baseY))), 1);
	const int baseYModulo = FMath::Max(FMath::Abs(GenerateSparsityModulo(baseY + (yCoefficient * baseX))), 1);

	const int xModulo = FMath::Clamp(baseXModulo - (baseXModulo % GRID_STEP), GRID_STEP, cSparsity - GRID_STEP);
	const int yModulo = FMath::Clamp(baseYModulo - (baseYModulo % GRID_STEP), GRID_STEP, cSparsity + GRID_STEP);

	return pixel.X % cSparsity == xModulo && pixel.Y % cSparsity == yModulo;
}

bool UPlanetProcGen::ShouldPixelContainPlanet(FIntPoint pixel)
{
	return ShouldGenerateObjectInLocationWithCoefficients(pixel, 3, 7);
}

bool UPlanetProcGen::ShouldPixelContainAsteroid(FIntPoint pixel)
{
	return ShouldGenerateObjectInLocationWithCoefficients(pixel, 6, 2);
}

void UPlanetProcGen::ProcGenAroundPlayer(FVector position)
{
	const int GRID_STEP = GetGridStep();

	// Convert the float vector to a pixel
	FIntPoint positionAsPixel{ FMath::FloorToInt(position.X), FMath::FloorToInt(position.Y) };

	int xMin = (positionAsPixel.X - (positionAsPixel.X % GRID_STEP)) - cSparsity;
	int xMax = (positionAsPixel.X + (positionAsPixel.X % GRID_STEP)) + cSparsity;
	int yMin = (positionAsPixel.Y - (positionAsPixel.Y % GRID_STEP)) - cSparsity;
	int yMax = (positionAsPixel.Y + (positionAsPixel.Y % GRID_STEP)) + cSparsity;

	for ( int row_index = xMin; row_index < xMax; row_index += GRID_STEP )
	{
		for ( int column_index = yMin; column_index < yMax; column_index += GRID_STEP )
		{
			FIntPoint gridPoint{ row_index, column_index };
			FVector location{ static_cast<float>(row_index) + 0.5f, static_cast<float>(column_index) + 0.5f, position.Z };

			if (blankPixels.Find(gridPoint) || planetData.Find(gridPoint))
			{
				continue;
			}
			else if (ShouldPixelContainPlanet( gridPoint ))
			{
				APlanet* newPlanet = GetWorld()->SpawnActor<APlanet>(defaultPlanetClass, location, FRotator::ZeroRotator);
				planetData.Add(gridPoint, newPlanet);
			}
			else
			{
				blankPixels.Add(gridPoint);

				if (ShouldPixelContainAsteroid(gridPoint))
				{
					AAsteroid* newAsteroid = GetWorld()->SpawnActor<AAsteroid>(defaultAsteroidClass, location, FRotator::ZeroRotator);
					newAsteroid->SetVelocity(FVector2D(position - location));
				}
			}
		}
	}
}

int UPlanetProcGen::GetGridStep() const
{
	return cSparsity / 100;
}

int UPlanetProcGen::GenerateSparsityModulo(int value) const
{
	for (int i = 0; i < 3; ++i)
	{
		value = (FMath::Square(value) + (3 * value) + 328479) % randomSeed;
	}

	return value % cSparsity;
}