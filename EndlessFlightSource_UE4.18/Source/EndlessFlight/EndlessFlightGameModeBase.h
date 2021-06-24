// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessFlightGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSFLIGHT_API AEndlessFlightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AEndlessFlightGameModeBase();

	void SpawnNextActor(FVector NextLocation);
	
protected:

	virtual void BeginPlay();

private:

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAcess = "true"))
	UClass* SpawningActorClass;
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAcess = "true"))
	FVector SpawningDistance;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAcess = "true"))
	USoundBase* ScoreSound;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAcess = "true"))
	USoundBase* GameOverSound;


	UPROPERTY(EditAnywhere, Category = "UMG", meta = (AllowPrivateAcess = "true"))
	TSubclassOf<class UUserWidget> MainWidgetClass;
	UPROPERTY()
	class UUserWidget* CurrentWidget;
	UPROPERTY()
	class UTextBlock* TextBlockScore;
	int32 ScoreValue;
	int32 HighScoreValue;

	UPROPERTY(EditAnywhere, Category = "UMG", meta = (AllowPrivateAcess = "true"))
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	UPROPERTY()
	class UUserWidget* GameOverWidget;
	class UTextBlock* TextBlockHighScore;
	class UTextBlock* TextBlockCurrentScore;
	class UButton* ButtonMenu;

	UPROPERTY(EditAnywhere, Category = "UMG", meta = (AllowPrivateAcess = "true"))
	TSubclassOf<class UUserWidget> MenuWidgetClass;
	class UUserWidget* MenuWidget;
	class UButton* ButtonPlay;
	class UButton* ButtonQuit;

	UFUNCTION()
	void ButtonMenuClicked();
	UFUNCTION()
	void ButtonPlayClicked();
	UFUNCTION()
	void ButtonQuitClicked();


	void SaveScore();
	void LoadScore();

public:

	bool bIsGameOver;

	void GameOver();
};
