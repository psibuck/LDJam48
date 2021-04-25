// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PlayerShip.generated.h"

UENUM()
enum E_Movement
{
	RotateLeft = 1 << 0,
	RotateRight = 1 << 1,
	ThrustUp = 1 << 2,
	ThrustDown = 1 << 3
};

DECLARE_DELEGATE_OneParam(FRotationInputComponent, const E_Movement);

UCLASS()
class LUDUMDARE48_API APlayerShip : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Refuel();

	UFUNCTION(BlueprintCallable)
	float GetFuelRemainingAsPercentage() const;

	UFUNCTION( BlueprintCallable )
	FString GetFuelString( const float fuel_remaining ) const;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Initial Settings")
	float RotationalSpeed{ 5.0f };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Initial Settings")
	float RocketPower{ 0.2f };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Fuel Settings")
	float MaxFuel{ 1000.0f };
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Fuel Settings")
	float FuelLevel{ 200.0f };

private:
	bool IsMovementFlagSet(const E_Movement flag) const;

	UFUNCTION()
	void SetMovementFlag(const E_Movement flag);
	
	UFUNCTION()
	void ClearMovementFlag(const E_Movement flag);

	float m_thrustLevel{ 0.0f };

	int8 m_rotationControl{ 0 };
};
