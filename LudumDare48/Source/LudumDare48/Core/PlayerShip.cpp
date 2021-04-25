// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"

// Sets default values
APlayerShip::APlayerShip()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerShip::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	float fuelToAdd{ 0.0f };
	if( FuelLevel > 0.0f )
	{
		float rotationToApply = 0.0f;
		if( IsShipStatusFlagSet( RotateLeft ) != IsShipStatusFlagSet( RotateRight ) )
		{

			if( IsShipStatusFlagSet( RotateRight ) )
			{
				AddControllerYawInput( RotationalSpeed * DeltaTime );
			}
			else
			{
				AddControllerYawInput( -RotationalSpeed * DeltaTime );
			}
		}
		if( IsShipStatusFlagSet( ThrustDown ) != IsShipStatusFlagSet( ThrustUp ) )
		{
			if( IsShipStatusFlagSet( ThrustUp ) )
			{
				m_thrustLevel += RocketPower;
			}
			else
			{
				m_thrustLevel = FMath::Max( 0.0f, m_thrustLevel - RocketPower );
			}

			fuelToAdd -= RocketBurnFuelCost;
		}
	}
	else
	{
		m_thrustLevel = 0.0f;
	}

	if( IsShipStatusFlagSet( Refuelling ) )
	{
		fuelToAdd += RefuelRate * DeltaTime;
	}

	FuelLevel += fuelToAdd * DeltaTime;

	AddMovementInput( GetActorForwardVector(), m_thrustLevel * DeltaTime );
}

// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAction<FRotationInputComponent>( "RotateLeft", IE_Pressed, this, &APlayerShip::SetShipStatusFlag, E_ShipStatus::RotateLeft );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "RotateLeft", IE_Released, this, &APlayerShip::ClearShipStatusFlag, E_ShipStatus::RotateLeft );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "RotateRight", IE_Pressed, this, &APlayerShip::SetShipStatusFlag, E_ShipStatus::RotateRight );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "RotateRight", IE_Released, this, &APlayerShip::ClearShipStatusFlag, E_ShipStatus::RotateRight );

	PlayerInputComponent->BindAction<FRotationInputComponent>( "ThrustUp", IE_Pressed, this, &APlayerShip::SetShipStatusFlag, E_ShipStatus::ThrustUp );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "ThrustUp", IE_Released, this, &APlayerShip::ClearShipStatusFlag, E_ShipStatus::ThrustUp );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "ThrustDown", IE_Pressed, this, &APlayerShip::SetShipStatusFlag, E_ShipStatus::ThrustDown );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "ThrustDown", IE_Released, this, &APlayerShip::ClearShipStatusFlag, E_ShipStatus::ThrustDown );
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
	return ( FuelLevel / MaxFuel ) * 100.0f;
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
	float percentage = GetFuelRemainingAsPercentage();

	if( use_hull )
	{
		percentage = GetHullIntegrityAsPercentage();
	}

	float  opacity = ( percentage - ( 100 - ( gage_section * 10 ) ) ) * 10;
	return opacity / 100;
}

float APlayerShip::GetHullIntegrityAsPercentage() const
{
	return m_currentHullIntegrity / MaxHullIntegrity * 100.0f;
}

void APlayerShip::ProcessAsteroidCollision()
{
	m_currentHullIntegrity = FMath::Max( 0.0f, m_currentHullIntegrity - AsteroidDamage );

	const FText hullMessage = FText::Format( FText::FromString( "Hull Level: {0}" ), m_currentHullIntegrity );
	GEngine->AddOnScreenDebugMessage( -1, 15.0f, FColor::Yellow, hullMessage.ToString() );

	if( m_currentHullIntegrity == 0 )
	{
		ProcessShipDeath();
	}
}

void APlayerShip::ProcessShipDeath()
{
	FuelLevel = 0.0f;

	GEngine->AddOnScreenDebugMessage( -1, 15.0f, FColor::Yellow, TEXT( "We are dead" ) );
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

