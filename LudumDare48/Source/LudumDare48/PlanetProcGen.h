// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Set.h"

#include "CoreMinimal.h"

#include "Math/IntPoint.h"
#include "Math/Vector2D.h"

#include "Planet.h"
#include "PlanetProcGen.generated.h"

UCLASS(Blueprintable)
class LUDUMDARE48_API UPlanetProcGen : public UObject
{
	GENERATED_BODY()

public:

	bool ShouldPixelContainPlanet(FIntPoint pixel);

	UFUNCTION(BlueprintCallable)
	void ProcGenAroundPlayer(FVector2D position);

	UPROPERTY(BlueprintReadWrite)
	TSet<FIntPoint> blankPixels;

	UPROPERTY(BlueprintReadWrite)
	int randomSeed{ 0 };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlanet> defaultPlanetClass;

private:

	UPROPERTY()
	TMap<FIntPoint, APlanet *> planetData;
};
