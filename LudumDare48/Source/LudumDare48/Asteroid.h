// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Math/Vector.h"

#include "Asteroid.generated.h"

UCLASS(Abstract)
class LUDUMDARE48_API AAsteroid : public AActor
{
	GENERATED_BODY()

public:
	AAsteroid();

	UFUNCTION( BlueprintImplementableEvent )
	void NotifySpeedChange();

	virtual void Tick(float deltaTime) override;

	void SetVelocity(FVector2D newVelocity);
	void SetSpeed( float newSpeed );

	FVector2D velocity;

	UPROPERTY( BlueprintReadWrite )
	float speed{ 1.0f };

	UPROPERTY( BlueprintReadWrite )
	float AsteroidScale{ 0.15f };
};
