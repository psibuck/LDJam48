// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PlayerShip.generated.h"

UENUM()
enum E_ShipStatus
{
	RotateLeft = 1 << 0,
	RotateRight = 1 << 1,
	ThrustUp = 1 << 2,
	ThrustDown = 1 << 3,
	Refuelling = 1 << 4
};

DECLARE_DELEGATE_OneParam( FRotationInputComponent, const E_ShipStatus );

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
	virtual void Tick( float DeltaTime ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	UFUNCTION( BlueprintImplementableEvent )
		void NotifyShipDeath();

	UFUNCTION( BlueprintCallable )
		void StartGame();

	UFUNCTION( BlueprintCallable )
		void StartRefuelling();

	UFUNCTION( BlueprintCallable )
		void StopRefuelling();

	UFUNCTION( BlueprintCallable )
		float GetFuelRemainingAsPercentage() const;

	UFUNCTION( BlueprintCallable )
		FString GetFuelString( const int fuel_remaining ) const;

	UFUNCTION( BlueprintCallable )
		float GetDistanceFromOrigin() const;

	UFUNCTION( BlueprintCallable )
		float GetFuelGageSectionOpacity( const int gage_section, bool use_hull ) const;

	UFUNCTION( BlueprintCallable )
		float GetThrustSectionOpacity( const int gage_section, bool is_reverse ) const;

	UFUNCTION( BlueprintCallable )
		float GetHullIntegrityAsPercentage() const;

	UFUNCTION(BlueprintCallable)
	float GetTimeAliveInSeconds() const { return m_startTime; }

	UFUNCTION( BlueprintCallable )
		float GetThrustPercentage() const { return m_thrustPercentage; }

	UFUNCTION( BlueprintCallable )
		void ProcessAsteroidCollision();

	UFUNCTION( BlueprintCallable )
		void SetShipName( FText shipName ) { m_shipName = shipName; }

protected:
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Initial Settings" )
		float RotationalSpeed{ 5.0f };

	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Initial Settings" )
		float MaxThrust{ 0.05f };

	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Fuel Settings" )
		float ThrustStepSize{ MaxThrust / 100.0f };

	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Fuel Settings" )
		float MaxFuel{ 1000.0f };

	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Fuel Settings" )
		float StartFuelLevel{ 200.0f };

	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Fuel Settings" )
		float RefuelRate{ 25.0f };

	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Fuel Settings" )
		float RocketBurnFuelCost{ 5.0f };

	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Initial Settings" )
		int AsteroidDamage{ 5 };

	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Initial Settings" )
		float StartHullIntegrity{ 100 };

	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Deepest Space | Initial Settings" )
		float MaxHullIntegrity{ 100 };

private:
	void FullThrust() { m_thrustPercentage = 100.0f;	}
	void FullReverse() { m_thrustPercentage = -100.0f; }
	void KillEngines() { m_thrustPercentage = 0.0f; }

	void ProcessShipDeath();

	bool IsShipStatusFlagSet( const E_ShipStatus flag ) const;

	UFUNCTION()
		void SetShipStatusFlag( const E_ShipStatus flag );

	UFUNCTION()
		void ClearShipStatusFlag( const E_ShipStatus flag );

	float m_thrustPercentage{ 0.0f };

	float m_currentHullIntegrity{ StartHullIntegrity };
	float m_currentFuelLevel{ StartFuelLevel };
	float m_startTime{ 0.0f };

	int8 m_shipStatusFlags{ 0 };

	FText m_shipName;
	bool m_alive{ false };
};
