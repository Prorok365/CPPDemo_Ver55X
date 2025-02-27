// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
ACoinManager::ACoinManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    totalMoney = 0;
    isTransactionSuccessful = false;
    currentCoinsCollected = 0;
	currentSpawnCoins = 2;
}

// Called when the game starts or when spawned
void ACoinManager::BeginPlay()
{
	Super::BeginPlay();
	
    // Find all Target Points in the level and store them
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), SpawnPoints);

	// Spawn coins
	SpawnCoins(); // Adjust number of coins as needed
}

// Called every frame
void ACoinManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ================== Custom Functions ================== //

/**
 * Adds a specified amount of money to the total balance.
 *
 * @param moneyToAdd - The amount of money to be added.
 *
 * Displays a debug message showing the updated total money.
 */
void ACoinManager::AddMoney(int32 moneyToAdd)
{
    // Add the specified amount of money to the total balance
    totalMoney += moneyToAdd;

    // Display the updated total money on the screen for debugging purposes
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Total money: %i"), totalMoney));

    CoinCounter(); 
}

/**
 * Attempts to make a purchase by deducting the required amount from the total money.
 *
 * @param RequiredMoney - The amount of money needed for the transaction.
 * @return true if the purchase is successful (enough funds available), false otherwise.
 *
 * Displays debug messages to indicate success or failure, along with the updated balance.
 */
bool ACoinManager::TryToPurchase(int32 RequiredMoney)
{
    // Check if the player has enough money to make the purchase
    if (totalMoney >= RequiredMoney)
    {
        // Deduct the required amount from the total balance
        totalMoney -= RequiredMoney;

        // Display success messages with updated total money
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Total Money = %i"), totalMoney));
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Transaction was successful!")));
        }

        return true; // Purchase was successful
    }
    else
    {
        // Display an error message if there is not enough money
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Not enough money!")));
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Total Money = %i"), totalMoney));
        }

        return false; // Purchase failed due to insufficient funds
    }
}


/**
 * 
 * 
 */
void ACoinManager::SpawnCoins()
{

    if (!CoinClass || SpawnPoints.Num() == 0) return; // Ensure we have a valid class and spawn points

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 i = 0; i < currentSpawnCoins; ++i)
    {
        if (SpawnPoints.IsValidIndex(i))
            GetWorld()->SpawnActor<AActor>(CoinClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
        else
        {
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("SpawnCoins: Out of array Boundaries!!")));
            
        }
    }

    if (SpawnTimer.IsValid())
    {
        SpawnTimer.Invalidate();
    }
}

/**
 *
 *
 */
void ACoinManager::CoinCounter()
{
    currentCoinsCollected++;
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Goal: %i / %i"), currentCoinsCollected, currentSpawnCoins));
    
    if (currentSpawnCoins < SpawnPoints.Num())
    {
        if (currentCoinsCollected >= currentSpawnCoins)
        {
            currentCoinsCollected = 0;
            currentSpawnCoins++;

            GetWorldTimerManager().SetTimer(SpawnTimer, this, &ACoinManager::SpawnCoins, 1.0f, false);

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Goal Reached!")));
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Goal: %i / %i"), currentCoinsCollected, currentSpawnCoins));
            }
        }
    }
}

// ====================================================== //