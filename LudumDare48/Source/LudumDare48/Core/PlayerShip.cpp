// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"

#include "HighScoreSave.h"
#include <Kismet/GameplayStatics.h>

const FString k_SaveGameSlot = "SaveGameSlot";

// Sets default values
APlayerShip::APlayerShip()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerShip::StartGame()
{
	m_currentHullIntegrity = StartHullIntegrity;
	m_currentFuelLevel = StartFuelLevel;
	m_alive = true;
	m_startTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	Restart();
	SetActorLocation(FVector(0, 0, 0));
}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	m_currentHullIntegrity = StartHullIntegrity;
	m_currentFuelLevel = StartFuelLevel;

	SetActorLocation(FVector(0, 0, 0));
}

// Called every frame
void APlayerShip::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (m_alive)
	{
		float fuelToAdd{ 0.0f };
		if (m_currentFuelLevel > 0.0f)
		{
			float rotationToApply = 0.0f;
			if (IsShipStatusFlagSet(RotateLeft) != IsShipStatusFlagSet(RotateRight))
			{

				if (IsShipStatusFlagSet(RotateRight))
				{
					AddControllerYawInput(RotationalSpeed * DeltaTime);
				}
				else
				{
					AddControllerYawInput(-RotationalSpeed * DeltaTime);
				}
			}
			if (IsShipStatusFlagSet(ThrustDown) != IsShipStatusFlagSet(ThrustUp))
			{
				if (IsShipStatusFlagSet(ThrustUp))
				{
					m_thrustPercentage = FMath::Min(100.0f, m_thrustPercentage + 1.0f);
				}
				else
				{
					m_thrustPercentage = FMath::Max(-100.0f, m_thrustPercentage - 1.0f); 
				}
			}
		}
		else
		{
			m_thrustPercentage = 0.0f;
		}

		fuelToAdd -= FMath::Abs(m_thrustPercentage) * RocketBurnFuelCost;

		if (IsShipStatusFlagSet(Refuelling))
		{
			fuelToAdd += RefuelRate;
		}

		m_currentFuelLevel = FMath::Max(0.0f, FMath::Min(MaxFuel, m_currentFuelLevel + fuelToAdd * DeltaTime));

		if (m_currentFuelLevel <= 0.0f)
		{
			ProcessShipDeath();
		}
		AddMovementInput(GetActorForwardVector(), m_thrustPercentage * MaxThrust * DeltaTime);
	}
}

// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<FRotationInputComponent>("RotateLeft", IE_Pressed, this, &APlayerShip::SetShipStatusFlag, E_ShipStatus::RotateLeft);
	PlayerInputComponent->BindAction<FRotationInputComponent>("RotateLeft", IE_Released, this, &APlayerShip::ClearShipStatusFlag, E_ShipStatus::RotateLeft);
	PlayerInputComponent->BindAction<FRotationInputComponent>("RotateRight", IE_Pressed, this, &APlayerShip::SetShipStatusFlag, E_ShipStatus::RotateRight);
	PlayerInputComponent->BindAction<FRotationInputComponent>("RotateRight", IE_Released, this, &APlayerShip::ClearShipStatusFlag, E_ShipStatus::RotateRight);

	PlayerInputComponent->BindAction<FRotationInputComponent>("ThrustUp", IE_Pressed, this, &APlayerShip::SetShipStatusFlag, E_ShipStatus::ThrustUp);
	PlayerInputComponent->BindAction<FRotationInputComponent>("ThrustUp", IE_Released, this, &APlayerShip::ClearShipStatusFlag, E_ShipStatus::ThrustUp);
	PlayerInputComponent->BindAction<FRotationInputComponent>("ThrustDown", IE_Pressed, this, &APlayerShip::SetShipStatusFlag, E_ShipStatus::ThrustDown);
	PlayerInputComponent->BindAction<FRotationInputComponent>("ThrustDown", IE_Released, this, &APlayerShip::ClearShipStatusFlag, E_ShipStatus::ThrustDown);

	PlayerInputComponent->BindAction("FullThrust", IE_Pressed, this, &APlayerShip::FullThrust);
	PlayerInputComponent->BindAction("FullReverse", IE_Pressed, this, &APlayerShip::FullReverse);
	PlayerInputComponent->BindAction("KillEngines", IE_Pressed, this, &APlayerShip::KillEngines);
}

void APlayerShip::StartRefuelling()
{
	SetShipStatusFlag( Refuelling );
}

void APlayerShip::StopRefuelling()
{
	ClearShipStatusFlag( Refuelling );
}

float APlayerShip::GetFuelRemainingAsPercentage() const
{
	return (m_currentFuelLevel / MaxFuel ) * 100.0f;
}

FString APlayerShip::GetFuelString( const int fuel_remaining ) const
{
	return FString::FromInt( fuel_remaining ) += "%";
}

float APlayerShip::GetDistanceFromOrigin() const
{
	return FVector::Dist2D( GetActorLocation(), FVector( 0, 0, 0 ) ) / 90;
}

float APlayerShip::GetFuelGageSectionOpacity( const int gage_section, bool use_hull ) const
{
	float percentage = FMath::Abs(GetFuelRemainingAsPercentage());

	if( use_hull )
	{
		percentage = GetHullIntegrityAsPercentage();
	}

	float opacity = ( percentage - ( 100 - ( gage_section * 10 ) ) ) * 10;
	return opacity / 100;
}

float APlayerShip::GetThrustSectionOpacity( const int gage_section, bool is_reverse ) const
{
	if( GetThrustPercentage() > -1 && is_reverse )
	{
		return 0;
	}

	if( GetThrustPercentage() < 1 && !is_reverse )
	{
		return 0;
	}

	float percentage = FMath::Abs( GetThrustPercentage() );
	float opacity = ( percentage - ( 100 - ( gage_section * 10 ) ) ) * 10;
	return opacity / 100;
}


float APlayerShip::GetHullIntegrityAsPercentage() const
{
	return m_currentHullIntegrity / MaxHullIntegrity * 100.0f;
}

void APlayerShip::ProcessAsteroidCollision()
{
	m_currentHullIntegrity = FMath::Max( 0.0f, m_currentHullIntegrity - AsteroidDamage );

	if( m_currentHullIntegrity == 0 )
	{
		ProcessShipDeath();
	}
}

void APlayerShip::ProcessShipDeath()
{
	UHighScoreSave* currentHighScore = Cast<UHighScoreSave>(UGameplayStatics::LoadGameFromSlot(k_SaveGameSlot, 0));
	if (!currentHighScore || GetDistanceFromOrigin() > currentHighScore->FurthestDistance)
	{
		UHighScoreSave* newSave = Cast<UHighScoreSave>(UGameplayStatics::CreateSaveGameObject(UHighScoreSave::StaticClass()));
		if (newSave)
		{
			newSave->PlayerName = m_shipName;
			newSave->FurthestDistance = GetDistanceFromOrigin();
			newSave->Position = GetActorLocation();
			newSave->TimeAliveSeconds = UGameplayStatics::GetRealTimeSeconds(GetWorld()) - m_startTime;
		}
		UGameplayStatics::SaveGameToSlot(newSave, k_SaveGameSlot, 0);
	}


	m_alive = false;
	m_currentFuelLevel = 0.0f;
	m_thrustPercentage = 0.0f;
	m_startTime = 0.0f;

	NotifyShipDeath();
}

bool APlayerShip::IsShipStatusFlagSet( const E_ShipStatus flag ) const
{
	return ( m_shipStatusFlags & static_cast<E_ShipStatus>( flag ) ) == static_cast<E_ShipStatus>( flag );
}

void APlayerShip::SetShipStatusFlag( const E_ShipStatus flag )
{
	m_shipStatusFlags |= flag;
}

void APlayerShip::ClearShipStatusFlag( const E_ShipStatus flag )
{
	m_shipStatusFlags &= ~( flag );
}

