// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Set.h"

#include "CoreMinimal.h"

#include "Math/IntPoint.h"
#include "Math/Vector.h"

#include "Planet.h"
#include "PlanetProcGen.generated.h"

UCLASS(Blueprintable)
class LUDUMDARE48_API UPlanetProcGen : public UObject
{
	GENERATED_BODY()

public:

	bool ShouldPixelContainPlanet(FIntPoint pixel);

	UFUNCTION(BlueprintCallable)
	void ProcGenAroundPlayer(FVector position);

	UPROPERTY(BlueprintReadWrite)
	TSet<FIntPoint> blankPixels;

	UPROPERTY(BlueprintReadWrite)
	int randomSeed{ 37242047 };

	UPROPERTY(EditDefaultsOnly)
	int cSparsity{ 100 };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlanet> defaultPlanetClass;

private:

	int GetGridStep() const;
	int GenerateSparsityModulo(int value) const;

	UPROPERTY()
	TMap<FIntPoint, APlanet *> planetData;
};
