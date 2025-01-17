// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unreal_Assignment3Character.h"
#include "Unreal_Assignment3PlayerController.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class UNREAL_ASSIGNMENT3_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	//Enemies
	UPROPERTY(BlueprintReadWrite)
		USkeletalMeshComponent* SkeletalMesh;

	float HitValue;

	UFUNCTION(BlueprintCallable)
		void OnHit();

	/** Enemy Health **/
	UPROPERTY(BlueprintReadOnly)
		float Enemy_HP = 1.0f;

	//#### Ultimate Ability player's reward rate
	const float ULTIMATE_REWARD = 0.2;
	void UltimateAbilityReward();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void onAttack();

	//#### Blueprint Event to Play the Animation Montage
	UFUNCTION(BlueprintImplementableEvent)
		void AttackEvent();


	UFUNCTION(BlueprintImplementableEvent)
		void Detected();
};
