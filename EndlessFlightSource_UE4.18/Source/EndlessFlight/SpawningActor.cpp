// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawningActor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BasePawn.h"
#include "EndlessFlightGameModeBase.h"
#include "Engine/World.h"


// Sets default values
ASpawningActor::ASpawningActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetArrowColor(FLinearColor::Green);

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	BaseMeshComp->SetupAttachment(ArrowComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(BaseMeshComp);

	FallLocation = FVector(0.0f, 0.0f, -100.0f);

}

// Called when the game starts or when spawned
void ASpawningActor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASpawningActor::OnComponentOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ASpawningActor::OnComponentOverlapEnd);

	FVector SpawnLocation = (GetActorLocation() + FVector(0.0f, -50.0f, -50.0f));

	if (!ObstacleActor) return;
	int32 SpawnedActors = 0;
	
	for (int32 i = 0; i < 9; i++)
	{

		FActorSpawnParameters SpawnParamters;


		if (FMath::RandRange(0, 3) != 0)
		{
			AActor* Spawned = GetWorld()->SpawnActor<AActor>(ObstacleActor, SpawnLocation, FRotator::ZeroRotator, SpawnParamters);
			Spawned->SetOwner(this);
			Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Spawned->SetLifeSpan(20.0f);

			SpawnedActors++;
		}

		if (SpawnLocation.Y >= 50.0f) {
			SpawnLocation.Z += 50.0f;
			SpawnLocation.Y = -50.0f;
		}
		else {

			SpawnLocation.Y += 50.0f;

		}

		if (SpawnedActors == 8)
			break;
	}


}

// Called every frame
void ASpawningActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASpawningActor::OnComponentOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (!Cast<ABasePawn>(OtherActor)) return;

	AEndlessFlightGameModeBase* GM = Cast<AEndlessFlightGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM)
		GM->SpawnNextActor(GetActorLocation());


	//SetLifeSpan(0.4f);
	//this->Destroy();
}

void ASpawningActor::OnComponentOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (!Cast<ABasePawn>(OtherActor)) return;

	AddActorLocalOffset(FallLocation);
	SetLifeSpan(0.5f);
}


