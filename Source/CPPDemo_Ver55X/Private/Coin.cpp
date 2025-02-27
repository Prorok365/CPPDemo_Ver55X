// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Engine.h"
#include "../CPPDemo_Ver55XCharacter.h"
#include "CoinManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CoinInit();

}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();

	// Set Visibility of the sphere collision component in and out of the game.
	SphereCollisionComponent->SetHiddenInGame(false);
	SphereCollisionComponent->SetVisibility(true);

	// Enable overlap events for the SphereCollisionComponent and bind the OnOverlapBegin and OnOverlapEnd functions
	// to handle events when another actor begins or ends overlap with the coin.
	SphereCollisionComponent->SetGenerateOverlapEvents(true);
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnOverlapBegin);
	SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACoin::OnOverlapEnd);
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// ================== Custom Functions ================== //

void ACoin::CoinInit()
{
	// Create the root component for the actor
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Create a static mesh component for the coin and attach it to the root component
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MyMesh->SetupAttachment(RootComponent);

	// Load the gold material asset from the starter content
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TorusAsset(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));

	// Check if the Mesh is valid and set it with right transform settings
	if (TorusAsset.Succeeded())
	{
		MyMesh->SetStaticMesh(TorusAsset.Object);
		MyMesh->SetRelativeLocation(FVector(0.0f, -15.0f, 0.0f));
		MyMesh->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
		MyMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	}

	// Load the gold material asset from the starter content
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GoldMaterial(TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'"));
	//Check and set the golden material
	if (GoldMaterial.Succeeded())
	{
		MyMesh->SetMaterial(0, GoldMaterial.Object);
	}


	// Create a sphere collision component for detecting overlaps and attach it to the static mesh
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollisionComponent->SetupAttachment(MyMesh);
	SphereCollisionComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	SphereCollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	SphereCollisionComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));

	// Create a rotating movement component for the coin and set the rotation rate
	MyRotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("rotatingMovementComponent"));
	MyRotatingMovementComponent->RotationRate = FRotator(0, -180, 0);
}


void ACoin::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is valid and not this actor
	if (OtherActor && OtherActor != this)
	{
		// Send a log with every actor that start overlap within the collision
		UE_LOG(LogTemp, Log, TEXT("Overlap Begin with: %s"), *OtherActor->GetName());

		// Attempt to cast the overlapping actor to ACPPDemoCharacter to check if it's the desired character type.
		// If the cast is successful, log a message to indicate an overlap with the character, including the character's name.
		ACPPDemo_Ver55XCharacter* Character = Cast<ACPPDemo_Ver55XCharacter>(OtherActor);

		/* 
			Guide: To get the CoinManager function we need to create a value reference of it, get the data about the class and only after
			that we can access to the coin manager functions, like AddMoney.
			Note:Its the SAME as we did in Blueprints, but more textual than visual.
		*/
		// Reference to the Coin Manager 
		ACoinManager* Ref_CM;

		// Attempt to get an instance of ACoinManager from the world
		Ref_CM = Cast<ACoinManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACoinManager::StaticClass()));

		if (Ref_CM) // Ensure the Coin Manager reference is valid
		{
			// Add the coin's value to the total money
			Ref_CM->AddMoney(coinValue);

			// Check if the overlapping character is valid
			if (Character != nullptr)
			{
				// Log a message indicating that the overlap with the character occurred
				UE_LOG(LogTemp, Warning, TEXT("Start Overlap with ThirdPersonCharacter: %s"), *Character->GetName());
			}

			// Destroy this actor (e.g., the coin) after being collected
			Destroy();
		}
	}
}

void ACoin::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the other actor is valid and not this actor
	if (OtherActor && OtherActor != this)
	{
		// Send a log with every actor that end overlap within the collision
		UE_LOG(LogTemp, Log, TEXT("Overlap End with: %s"), *OtherActor->GetName());

		// Attempt to cast the overlapping actor to ACPPDemoCharacter to check if it's the desired character type.
		// If the cast is successful, log a message to indicate an overlap with the character, including the character's name.
		ACPPDemo_Ver55XCharacter* Character = Cast<ACPPDemo_Ver55XCharacter>(OtherActor);
		if (Character != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("End Overlap with ThirdPersonCharacter: %s"), *Character->GetName());
		}
	}
}

// ====================================================== //
