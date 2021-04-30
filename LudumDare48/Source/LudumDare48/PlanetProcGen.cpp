// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetProcGen.h"

#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/UnrealMathUtility.h"

const FVector HOME_PLANET_LOCATION{ 100.0f , -245.0f, -250.0f };

bool UPlanetProcGen::ShouldGenerateObjectInLocationWithCoefficients(FIntPoint pixel, int xCoefficient, int yCoefficient, int sparsity)
{
	// Procedurally determines whether this pixel should contain an object given the following coefficients

	const int GRID_STEP = GetGridStep();

	const int baseX = FMath::Abs(pixel.X / sparsity);
	const int baseY = FMath::Abs(pixel.Y / sparsity);

	const int baseXModulo = FMath::Max(FMath::Abs(GenerateSparsityModulo(baseX + (xCoefficient * baseY), sparsity)), 1);
	const int baseYModulo = FMath::Max(FMath::Abs(GenerateSparsityModulo(baseY + (yCoefficient * baseX), sparsity)), 1);

	const int xModulo = FMath::Clamp(baseXModulo - (baseXModulo % GRID_STEP), GRID_STEP, sparsity - GRID_STEP);
	const int yModulo = FMath::Clamp(baseYModulo - (baseYModulo % GRID_STEP), GRID_STEP, sparsity + GRID_STEP);

	return pixel.X % sparsity == xModulo && pixel.Y % sparsity == yModulo;
}

bool UPlanetProcGen::ShouldPixelContainPlanet(FIntPoint pixel)
{
	return ShouldGenerateObjectInLocationWithCoefficients(pixel, 3, 7, cPlanetSparsity);
}

bool UPlanetProcGen::ShouldPixelContainAsteroid(FIntPoint pixel)
{
	return ShouldGenerateObjectInLocationWithCoefficients(pixel, 6, 2, cAsteroidSparsity);
}

void UPlanetProcGen::ProcGenAroundPlayer(FVector position)
{
	const int GRID_STEP = GetGridStep();

	// Convert the float vector to a pixel
	FIntPoint positionAsPixel{ FMath::FloorToInt(position.X), FMath::FloorToInt(position.Y) };

	int xMin = (positionAsPixel.X - (positionAsPixel.X % GRID_STEP)) - cPlanetSparsity;
	int xMax = (positionAsPixel.X + (positionAsPixel.X % GRID_STEP)) + cPlanetSparsity;
	int yMin = (positionAsPixel.Y - (positionAsPixel.Y % GRID_STEP)) - cPlanetSparsity;
	int yMax = (positionAsPixel.Y + (positionAsPixel.Y % GRID_STEP)) + cPlanetSparsity;

	for ( int row_index = xMin; row_index < xMax; row_index += GRID_STEP )
	{
		for ( int column_index = yMin; column_index < yMax; column_index += GRID_STEP )
		{
			FIntPoint gridPoint{ row_index, column_index };

			if( m_HomePlanet == nullptr )
			{
				m_HomePlanet = GetWorld()->SpawnActor<APlanet>( defaultPlanetClass, HOME_PLANET_LOCATION, FRotator::ZeroRotator );
				planetData.Add( gridPoint, m_HomePlanet );
				continue;
			}	

			FVector location{ static_cast<float>(row_index) + 0.5f, static_cast<float>(column_index) + 0.5f, position.Z };

			if (blankPixels.Find(gridPoint) || planetData.Find(gridPoint))
			{
				continue;
			}
			else if (ShouldPixelContainPlanet(gridPoint))
			{
				location += {0.0f, 0.0f, -250.0f};
				APlanet* newPlanet = GetWorld()->SpawnActor<APlanet>(defaultPlanetClass, location, FRotator::ZeroRotator);
				planetData.Add(gridPoint, newPlanet);
			}
			else
			{
				blankPixels.Add(gridPoint);

				if (ShouldPixelContainAsteroid(gridPoint))
				{
					AAsteroid* newAsteroid = GetWorld()->SpawnActor<AAsteroid>(defaultAsteroidClass, location, FRotator::ZeroRotator);
					newAsteroid->SetSpeed( FMath::RandRange( 40.0f, 150.0f ) );

					FVector2D newAsteroidVelocity{ FMath::RandPointInCircle(1.0f) };
					newAsteroid->SetVelocity(newAsteroidVelocity);
				}
			}
		}
	}
}

int UPlanetProcGen::GetGridStep() const
{
	return cPlanetSparsity / 10;
}

int UPlanetProcGen::GenerateSparsityModulo(int value, int sparsity) const
{
	for (int i = 0; i < 3; ++i)
	{
		value = (FMath::Square(value) + (3 * value) + 328479) % randomSeed;
	}

	return value % sparsity;
}