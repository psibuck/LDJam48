// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"
#include "Math/Vector.h"

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

	if( FuelLevel > 0.0f )
	{
		float rotationToApply = 0.0f;
		if( IsMovementFlagSet( RotateLeft ) != IsMovementFlagSet( RotateRight ) )
		{

			if( IsMovementFlagSet( RotateRight ) )
			{
				AddControllerYawInput( RotationalSpeed * DeltaTime );
			}
			else
			{
				AddControllerYawInput( -RotationalSpeed * DeltaTime );
			}
		}
		if( IsMovementFlagSet( ThrustDown ) != IsMovementFlagSet( ThrustUp ) )
		{
			if( IsMovementFlagSet( ThrustUp ) )
			{
				m_thrustLevel += RocketPower;
			}
			else
			{
				m_thrustLevel = FMath::Max( 0.0f, m_thrustLevel - RocketPower );
			}
		}
		FuelLevel = FMath::Max( 0.0f, FuelLevel - m_thrustLevel );
	}
	else
	{
		m_thrustLevel = 0.0f;
	}

	AddMovementInput( GetActorForwardVector(), m_thrustLevel * DeltaTime );

	/*const FText fuelMessage = FText::Format(FText::FromString("Fuel Level: {0}"), GetFuelRemainingAsPercentage());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, fuelMessage.ToString());*/
}

// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAction<FRotationInputComponent>( "RotateLeft", IE_Pressed, this, &APlayerShip::SetMovementFlag, E_Movement::RotateLeft );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "RotateLeft", IE_Released, this, &APlayerShip::ClearMovementFlag, E_Movement::RotateLeft );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "RotateRight", IE_Pressed, this, &APlayerShip::SetMovementFlag, E_Movement::RotateRight );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "RotateRight", IE_Released, this, &APlayerShip::ClearMovementFlag, E_Movement::RotateRight );

	PlayerInputComponent->BindAction<FRotationInputComponent>( "ThrustUp", IE_Pressed, this, &APlayerShip::SetMovementFlag, E_Movement::ThrustUp );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "ThrustUp", IE_Released, this, &APlayerShip::ClearMovementFlag, E_Movement::ThrustUp );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "ThrustDown", IE_Pressed, this, &APlayerShip::SetMovementFlag, E_Movement::ThrustDown );
	PlayerInputComponent->BindAction<FRotationInputComponent>( "ThrustDown", IE_Released, this, &APlayerShip::ClearMovementFlag, E_Movement::ThrustDown );
}

void APlayerShip::Refuel()
{
	FuelLevel += 40.0f;
}

float APlayerShip::GetFuelRemainingAsPercentage() const
{
	return ( FuelLevel / MaxFuel ) * 100.0f;
}

float APlayerShip::GetDistanceFromOrigin() const
{
	return FVector::Dist2D( GetActorLocation(), FVector( 0, 0, 0 ) ) / 90;
}

FString APlayerShip::GetFuelString( const int fuel_remaining ) const
{
	return FString::FromInt( fuel_remaining ) += "%";
}

float APlayerShip::GetFuelGageSectionOpacity( const int gage_section ) const
{
	const float fuel = GetFuelRemainingAsPercentage();
	float  opacity = ( fuel - ( 100 - ( gage_section * 10 ) ) ) * 10;
	return opacity / 100;
}

bool APlayerShip::IsMovementFlagSet( const E_Movement flag ) const
{
	return ( m_rotationControl & static_cast<E_Movement>( flag ) ) == static_cast<E_Movement>( flag );
}

void APlayerShip::SetMovementFlag( const E_Movement flag )
{
	m_rotationControl |= flag;
}

void APlayerShip::ClearMovementFlag( const E_Movement flag )
{
	m_rotationControl &= ~( flag );
}

