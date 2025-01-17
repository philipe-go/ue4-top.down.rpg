// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_Assignment3PlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Unreal_Assignment3Character.h"
#include "Engine/World.h"
#include "Lootbox.h"
#include "MySaveGame.h"

AUnreal_Assignment3PlayerController::AUnreal_Assignment3PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AUnreal_Assignment3PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}

	
}

void AUnreal_Assignment3PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AUnreal_Assignment3PlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AUnreal_Assignment3PlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AUnreal_Assignment3PlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AUnreal_Assignment3PlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnreal_Assignment3PlayerController::OnResetVR);
	// support touch devices 
	InputComponent->BindAction("Shoot", IE_Pressed, this, &AUnreal_Assignment3PlayerController::OnShoot);
	InputComponent->BindAction("LoseHP", IE_Pressed, this, &AUnreal_Assignment3PlayerController::LoseHP);
	InputComponent->BindAction("AOE", IE_Pressed, this, &AUnreal_Assignment3PlayerController::onAOE);

	InputComponent->BindAction("Action", IE_Pressed, this, &AUnreal_Assignment3PlayerController::OnAction);
	InputComponent->BindAction("HealthPotion", IE_Pressed, this, &AUnreal_Assignment3PlayerController::OnHPPotion);
	InputComponent->BindAction("ManaPotion", IE_Pressed, this, &AUnreal_Assignment3PlayerController::OnManaPotion);
	InputComponent->BindAction("SpeedPotion", IE_Pressed, this, &AUnreal_Assignment3PlayerController::OnSpeedPotion);

	InputComponent->BindAction("UltimateAbility", IE_Pressed, this, &AUnreal_Assignment3PlayerController::OnUltimateAbility);
}

void AUnreal_Assignment3PlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnreal_Assignment3PlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AUnreal_Assignment3Character* MyPawn = Cast<AUnreal_Assignment3Character>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AUnreal_Assignment3PlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AUnreal_Assignment3PlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AUnreal_Assignment3PlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AUnreal_Assignment3PlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

// To Shoot projectiles
void AUnreal_Assignment3PlayerController::OnShoot()
{
	AUnreal_Assignment3Character* MyCharacter = Cast<AUnreal_Assignment3Character>(GetPawn());
	//When the player spawns a projectile, it will use 10% of the mana
	
	if (MyCharacter->Mana > 0)
	{

		

		// Print String here :) 
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Blue, TEXT("Pew"));
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			FVector Direction = Hit.ImpactPoint - MyCharacter->GetActorLocation();
			Direction.Z = 0;
			MyCharacter->SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
		}

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyCharacter->GetActorLocation());
		MyCharacter->Shoot();

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Blue, TEXT("Out of Mana!"));
	}
	
}

// Bind key to lose HP
void AUnreal_Assignment3PlayerController::LoseHP()
{
	AUnreal_Assignment3Character* MyPawn = Cast<AUnreal_Assignment3Character>(GetPawn());
	if (MyPawn != nullptr)
	{
		MyPawn->HP -= 0.1f;
	}

}

void AUnreal_Assignment3PlayerController::onAOE()
{
	
	AUnreal_Assignment3Character* MyCharacter = Cast<AUnreal_Assignment3Character>(GetPawn());

	//If Mana is zero, the player cannot spawn spells
	if (MyCharacter->Mana > 0)
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, TEXT("BOOM!"));
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			FVector Direction = Hit.ImpactPoint - MyCharacter->GetActorLocation();
			Direction.Z = 0;
			MyCharacter->SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
		}

		MyCharacter->AOE();
		MyCharacter->Mana -= 0.2f;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Blue, TEXT("Out of Mana!"));
	}
}

void AUnreal_Assignment3PlayerController::OnAction()
{
	AUnreal_Assignment3Character* MyPlayer = Cast<AUnreal_Assignment3Character>(GetPawn());

	if (MyPlayer)
	{
		//### Bind the action to the lootbox methods.
		auto* TempActor = Cast<ALootbox>(MyPlayer->OnActionReceiver);
		if (TempActor) { TempActor->OpenLoot(); }
	}
}

void AUnreal_Assignment3PlayerController::OnHPPotion()
{
	AUnreal_Assignment3Character* MyPlayer = Cast<AUnreal_Assignment3Character>(GetPawn());

	if (MyPlayer) { MyPlayer->UseHPPotion(); }
}

void AUnreal_Assignment3PlayerController::OnManaPotion()
{
	AUnreal_Assignment3Character* MyPlayer = Cast<AUnreal_Assignment3Character>(GetPawn());

	if (MyPlayer) { MyPlayer->UseManaPotion(); }
}

void AUnreal_Assignment3PlayerController::OnSpeedPotion()
{
	AUnreal_Assignment3Character* MyPlayer = Cast<AUnreal_Assignment3Character>(GetPawn());

	if (MyPlayer) { MyPlayer->UseSpeedPotion(); }
}

void AUnreal_Assignment3PlayerController::OnUltimateAbility()
{
	AUnreal_Assignment3Character* MyPlayer = Cast<AUnreal_Assignment3Character>(GetPawn());
	if (MyPlayer)
	{
		MyPlayer->UseUltimateAbility();
	}
}
