// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "CoinManager.h"
#include "../CPPDemo_Ver55XCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"


// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	DoorInit();



	// Create the Timeline Component
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Door Timeline"));
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	// Set Visibility of the sphere collision component in and out of the game.
	BoxCollisionComponent->SetHiddenInGame(false);
	BoxCollisionComponent->SetVisibility(true);

	// Enable overlap events for the BoxCollisionComponent and bind the OnOverlapBegin and OnOverlapEnd functions
	// to handle events when another actor begins or ends overlap with the Door.
	BoxCollisionComponent->SetGenerateOverlapEvents(true);
	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	BoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnOverlapEnd);



	// Get the door's original relative rotation
	InitialDoorRotation = DoorBody->GetRelativeRotation();

	// Set target rotation (rotating 90 degrees around the Z-axis)
	TargetDoorRotation = InitialDoorRotation + FRotator(0, 90, 0);

	if (DoorCurve)
	{
		// Bind function to update event
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("TimelineUpdate"));

		// Bind function to finished event
		FOnTimelineEvent FinishFunction;
		FinishFunction.BindUFunction(this, FName("TimelineFinished"));

		// Add functions to Timeline
		DoorTimeline->AddInterpFloat(DoorCurve, ProgressFunction);
		DoorTimeline->SetTimelineFinishedFunc(FinishFunction);
	}
}


// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ADoor::DoorInit()
{
	// Create the root component for the actor
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Create a static mesh component for the door and attach it to the root component
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	DoorFrame->SetupAttachment(RootComponent);

	// Create a static mesh component for the door body and attach it to the Door frame
	DoorBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Body"));
	DoorBody->SetupAttachment(DoorFrame);

	// Load the Door Frame asset from the starter content
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorFrameAsset(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_DoorFrame.SM_DoorFrame'"));
	// Load the Door Body asset from the starter content
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorBodyAsset(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_Door.SM_Door'"));

	// Check if the Meshes is valid and set it with right transform settings
	if (DoorFrameAsset.Succeeded() && DoorBodyAsset.Succeeded())
	{
		DoorFrame->SetStaticMesh(DoorFrameAsset.Object);
		DoorBody->SetStaticMesh(DoorBodyAsset.Object);

		DoorBody->SetRelativeLocation(FVector(0.0f, 45.0f, 0.0f));
	}

	// Create a box collision component for detecting overlaps and attach it to the static mesh
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	if (BoxCollisionComponent)
	{
		BoxCollisionComponent->SetupAttachment(DoorFrame);
		BoxCollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
		BoxCollisionComponent->SetWorldScale3D(FVector(4.75f, 2.25f, 3.5f));
	}
}

// ================== Custom Functions ================== //

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is valid and not this actor
	if (OtherActor && OtherActor != this)
	{
		// Send a log with every actor that start overlap within the collision
		UE_LOG(LogTemp, Log, TEXT("Overlap Begin with: %s"), *OtherActor->GetName());

		// Attempt to cast the overlapping actor to ACPPDemoCharacter to check if it's the desired character type.
		// If the cast is successful, log a message to indicate an overlap with the character, including the character's name.
		ACPPDemo_Ver55XCharacter* Character = Cast<ACPPDemo_Ver55XCharacter>(OtherActor);

		//To get the CoinManager function we need to create a value reference of it, 
		// get the data about the class and only after that we can access to the coin manager functions.
		ACoinManager* Ref_CM;
		Ref_CM = Cast<ACoinManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACoinManager::StaticClass()));

		if (Ref_CM != nullptr)
		{
			if (bIsUnlocked)
			{
				DoorTimeline->Play();
			}
			else
			{
				if (Ref_CM->TryToPurchase(requiredMoney))
				{
					bIsUnlocked = true;
					DoorTimeline->Play();
				}
			}
			
			if (Character != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Start Overlap with ThirdPersonCharacter: %s"), *Character->GetName());
			}
		}
	}
}

void ADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the other actor is valid and not this actor
	if (OtherActor && OtherActor != this)
	{
		// Send a log with every actor that end overlap within the collision
		UE_LOG(LogTemp, Log, TEXT("Overlap End with: %s"), *OtherActor->GetName());

		// Close the door when end the overlap
		DoorTimeline->Reverse();

		// Attempt to cast the overlapping actor to ACPPDemoCharacter to check if it's the desired character type.
		// If the cast is successful, log a message to indicate an overlap with the character, including the character's name.
		ACPPDemo_Ver55XCharacter* Character = Cast<ACPPDemo_Ver55XCharacter>(OtherActor);
		if (Character != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Start Overlap with ThirdPersonCharacter: %s"), *Character->GetName());
		}
	}
}


void ADoor::TimelineUpdate(float Value)
{
	// Interpolating between the initial and target positions
	FRotator NewRotation = FMath::Lerp(InitialDoorRotation, TargetDoorRotation, Value);
	DoorBody->SetRelativeRotation(NewRotation);
}

void ADoor::TimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Door animation finished"));
}


// ====================================================== //