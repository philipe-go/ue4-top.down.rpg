// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class UNREAL_ASSIGNMENT3_API AEnemySpawner : public AActor
{
#pragma region Default fields
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


#pragma endregion 

#pragma region Custom fields
public:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TrapMesh;

	UPROPERTY(EditAnywhere)
	FBox SpawnBounds;

	UPROPERTY(EditAnywhere)
		 class ANavigationData* NavMeshAgent;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//### Enemy type to be spawned in the trap area. Enemy should be added in the Editor
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> EnemyToSpawn;

	//### How many Enemies will be spawned
	UPROPERTY(EditAnywhere)
		int SpawnAmount = 3;

	//### Distance from player where the enemies can spawn
	UPROPERTY(EditAnywhere)
		float SpawnRadius = 600;

	//### Custom event to spawn Niagara System at each spawned enemy's location
	UFUNCTION(BlueprintImplementableEvent)
		void OnEnemySpawn(FVector loc, FRotator rot);

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* TriggerArea;

	void SpawnEnemies(AActor* MyPlayer);
#pragma endregion 
};
