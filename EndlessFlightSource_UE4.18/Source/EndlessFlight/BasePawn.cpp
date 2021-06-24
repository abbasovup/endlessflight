// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "EndlessFlightGameModeBase.h"




// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	RotorMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotorMeshComp"));
	RotorMeshComp->SetupAttachment(MeshComp);


	MaxRadius = 100.0f;
	MovementDistance = 100.0f;
	MovementSpeed = 100.0f;
	SpeedRate = 1.0f;
	RotorSpeedRate = 1.0f;
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), CameraActors);

	
	for (int32 i = 0; i < CameraActors.Num(); i++)
	{
		if (CameraActors[i]->ActorHasTag("CameraActor")) {

			GameplayCamera = Cast<ACameraActor>(CameraActors[i]);
			if (GameplayCamera) {

				APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
				if (PC) {

					PC->SetViewTarget(GameplayCamera);
					UE_LOG(LogTemp, Warning, TEXT("SetViewTarget IS WORKING!"));
				}
			}


			break;
		}
	}

	

	GM = Cast<AEndlessFlightGameModeBase>(GetWorld()->GetAuthGameMode());

	bIsGameOver = false;
	bIsGameStarted = false;
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGameOver || !bIsGameStarted) return;

	MovementSpeed += DeltaTime * SpeedRate;
	MoveForward(DeltaTime);
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &ABasePawn::MoveUp);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ABasePawn::MoveDown);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABasePawn::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ABasePawn::MoveRight);
}

void ABasePawn::MoveForward(float DeltaTime)
{
	AddActorLocalOffset(FVector(MovementSpeed * DeltaTime, 0.0f, 0.0f));
	
	if (GameplayCamera) {
		GameplayCamera->AddActorLocalOffset(FVector(MovementSpeed * DeltaTime, 0.0f, 0.0f));
	}

	RotorMeshComp->AddLocalRotation(FRotator(0.0f, MovementSpeed * RotorSpeedRate * DeltaTime, 0.0f));

}

void ABasePawn::MoveUp()
{
	if (GetActorLocation().Z >= MaxRadius)
		return;

	AddActorLocalOffset(FVector(0.0f, 0.0f, MovementDistance));
}

void ABasePawn::MoveDown()
{
	if (GetActorLocation().Z <= -MaxRadius)
		return;

	AddActorLocalOffset(FVector(0.0f, 0.0f, -MovementDistance));
}

void ABasePawn::MoveLeft()
{
	if (GetActorLocation().Y <= -MaxRadius)
		return;

	AddActorLocalOffset(FVector(0.0f, -MovementDistance, 0.0f));
}

void ABasePawn::MoveRight()
{
	if (GetActorLocation().Y >= MaxRadius)
		return;

	AddActorLocalOffset(FVector(0.0f, MovementDistance, 0.0f));
}

void ABasePawn::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (bIsGameOver) return;

	if (OtherActor->ActorHasTag(FName("Obstruction"))) {
		UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
		
		if (GM)
			GM->GameOver();

		bIsGameOver = true;
	}
}

