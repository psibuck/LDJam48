// Fill out your copyright notice in the Description page of Project Settings.

#include "Asteroid.h"

// Sets default values
AAsteroid::AAsteroid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAsteroid::Tick( float deltaTime )
{
	FVector2D change = velocity * speed * deltaTime;
	SetActorLocation( GetActorLocation() + FVector( change.X, change.Y, 0.0f ) );

	Super::Tick( deltaTime );
}

void AAsteroid::SetSpeed( float newSpeed )
{
	speed = newSpeed;
	AsteroidScale = speed / 1000;

	if( speed > 90 )
	{
		AsteroidScale -= 0.20f;
	}
	else
	{
		AsteroidScale += 0.10f;
	}

	NotifySpeedChange();
}

void AAsteroid::SetVelocity( FVector2D newVelocity )
{
	velocity = newVelocity;
}