// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"
#include "TimerManager.h"
#include "CoinManager.generated.h"



UCLASS()
class CPPDEMO_VER55X_API ACoinManager : public AActor
{
	GENERATED_BODY()

private:

	// ================== Properties Declarations ================== //

	UPROPERTY(VisibleInstanceOnly, Category = "Coins")
	int32 totalMoney = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Interactions")
	bool isTransactionSuccessful = false;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TArray<AActor*> SpawnPoints; // Holds Target Points

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<AActor> CoinClass; // Coin class to spawn

	UPROPERTY(VisibleInstanceOnly, Category = "Points")
	int32 currentCoinsCollected = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Points")
	int32 currentSpawnCoins = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	FTimerHandle SpawnTimer;

	// ============================================================ //

public:	
	// Sets default values for this actor's properties
	ACoinManager();

	// ================== Functions Declarations ================== //

	UFUNCTION(BlueprintCallable, Category = "Coins")
	void AddMoney(int32 moneyToAdd);

	UFUNCTION(BlueprintCallable, Category = "Coins")
	FORCEINLINE int32 GetMoney()
	{
		return totalMoney;
	}

	UFUNCTION(BlueprintCallable, Category = "Interactions")
	bool TryToPurchase(int32 RequiredMoney);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnCoins();

	UFUNCTION(BlueprintCallable, Category = "Points")
	void CoinCounter();

	// ============================================================ //

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
