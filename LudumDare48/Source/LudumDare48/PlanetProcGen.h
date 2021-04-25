// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Set.h"

#include "CoreMinimal.h"

#include "Math/IntPoint.h"
#include "Math/Vector.h"

#include "Asteroid.h"
#include "Planet.h"

#include "PlanetProcGen.generated.h"

UCLASS(Blueprintable)
class LUDUMDARE48_API UPlanetProcGen : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void ProcGenAroundPlayer(FVector position);

	UPROPERTY(BlueprintReadWrite)
	TSet<FIntPoint> blankPixels;

	UPROPERTY(BlueprintReadWrite)
	int randomSeed{ 37242047 };

	UPROPERTY(EditDefaultsOnly)
	int cPlanetSparsity{ 100 };

	UPROPERTY(EditDefaultsOnly)
	int cAsteroidSparsity{ 100 };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlanet> defaultPlanetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAsteroid> defaultAsteroidClass;

private:

	bool ShouldGenerateObjectInLocationWithCoefficients(FIntPoint pixel, int xCoefficient, int yCoefficient, int sparsity);
	bool ShouldPixelContainPlanet(FIntPoint pixel);
	bool ShouldPixelContainAsteroid(FIntPoint pixel);

	int GetGridStep() const;
	int GenerateSparsityModulo(int value, int sparsity) const;

	UPROPERTY()
	TMap<FIntPoint, APlanet *> planetData;
};
