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

	virtual void Tick(float deltaTime) override;

	void SetVelocity(FVector2D newVelocity);

	FVector2D velocity;

	UPROPERTY(EditDefaultsOnly)
	float speed{ 1.0f };
};
