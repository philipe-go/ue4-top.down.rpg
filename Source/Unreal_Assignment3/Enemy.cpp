// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/Character.h"
#include "Unreal_Assignment3Character.h"
#include "Unreal_Assignment3PlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemy::OnHit()
{
	HitValue = 1;
    
	// When the enemy gets hit, he loses 10%
	Enemy_HP -= 0.1f;
	UltimateAbilityReward();

	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("Enemy is HIT"));
}


void AEnemy::UltimateAbilityReward()
{
	//#### Code to increase the UltimaAbility Player's property when an Enemy is killed
	AUnreal_Assignment3Character* MyPlayer = Cast<AUnreal_Assignment3Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (MyPlayer)
	{
		MyPlayer->UltimateAbility += ULTIMATE_REWARD;
		FMath::Clamp(MyPlayer->UltimateAbility, 0.0f, 1.0f);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	// LERP -> Color goes back to white
	HitValue -= DeltaTime;
	if (HitValue < 0)
	{
		HitValue = 0;
	}
	SkeletalMesh->SetScalarParameterValueOnMaterials(TEXT("Hit"), HitValue);

	// Enemy Dies if their HP < 0
	if (Enemy_HP <= 0)
	{
		// Cast to Player
		AUnreal_Assignment3Character* MyPlayer = Cast<AUnreal_Assignment3Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
		
		// This should give the player XP
		MyPlayer->GiveXP();

		// Checks Lvl System
		MyPlayer->LevelUpSystem();
	
		SkeletalMesh->SetScalarParameterValueOnMaterials(TEXT("Enemy is DEAD!"), HitValue);
		Destroy();
	}

	// CHECK DISTANCE BETWEEN PLAYER AND ENEMY
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector EnemyPos = GetActorLocation();

	if (FVector::Dist(PlayerLocation, EnemyPos) <= 400)
	{
		onAttack();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::onAttack()
{
	GEngine->AddOnScreenDebugMessage(0, 0, FColor::Red, TEXT("ANGRY ENEMY!"));

	Detected();
}






