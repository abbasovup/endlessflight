// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawningActor.generated.h"

UCLASS()
class ENDLESSFLIGHT_API ASpawningActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawningActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAcess = "true"))
	class UArrowComponent* ArrowComp;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAcess = "true"))
	class UStaticMeshComponent* BaseMeshComp;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAcess = "true"))
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAcess = "true"))
	UClass* ObstacleActor;
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAcess = "true"))
	FVector FallLocation;

public:
	UFUNCTION()
	void OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
