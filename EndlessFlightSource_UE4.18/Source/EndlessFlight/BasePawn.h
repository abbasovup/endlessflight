// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class ENDLESSFLIGHT_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComp;
	UPROPERTY(EditAnywhere, Category = "Components", meta =(AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RotorMeshComp;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class ACameraActor* GameplayCamera;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaxRadius;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementDistance;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SpeedRate;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotorSpeedRate;

	class AEndlessFlightGameModeBase* GM;
	bool bIsGameOver;

	UFUNCTION()
	void MoveForward(float DeltaTime);
	UFUNCTION()
	void MoveUp();
	UFUNCTION()
	void MoveDown();
	UFUNCTION()
	void MoveLeft();
	UFUNCTION()
	void MoveRight();

public:

	bool bIsGameStarted;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
