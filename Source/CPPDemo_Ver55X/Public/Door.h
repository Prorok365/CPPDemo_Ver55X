// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"



UCLASS()
class CPPDEMO_VER55X_API ADoor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Requirements")
	int32 requiredMoney = 10;

public:	
	// Sets default values for this actor's properties
	ADoor();

	// ================== Properties Declarations ================== //

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* DoorFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* DoorBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxCollisionComponent;



	// Timeline to control the animation
	UPROPERTY()
	UTimelineComponent* DoorTimeline;

	// Curve used to drive the timeline
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* DoorCurve;

	// Original door rotation
	FRotator InitialDoorRotation;

	// Target rotation to open the door
	FRotator TargetDoorRotation;

	// Boolean to track if the transaction works once
	bool bIsUnlocked = false;

	// ============================================================ //

	// ================== Functions Declarations ================== //

	UFUNCTION(BlueprintCallable, Category = "Init")
	void DoorInit();

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Function to be called on Timeline Tick
	UFUNCTION(BlueprintCallable, Category = "Timeline")
	void TimelineUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Timeline")
	void TimelineFinished();

	// ============================================================ //

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
