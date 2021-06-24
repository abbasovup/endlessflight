// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessFlightGameModeBase.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "BasePawn.h"



AEndlessFlightGameModeBase::AEndlessFlightGameModeBase() {

	SpawningDistance = FVector(600.0f, 0.0f, 0.0f);

	bIsGameOver = false;
	HighScoreValue = 0;
}


void AEndlessFlightGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ScoreValue = 0;

	if (MainWidgetClass) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
		if (CurrentWidget) {
			CurrentWidget->AddToViewport();
			TextBlockScore = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("TextBlock_Score")));

			if(TextBlockScore) {
				TextBlockScore->SetText(FText::FromString(FString::FromInt(ScoreValue)));
			}
		}
	}

	if (GameOverWidgetClass) {

		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (GameOverWidget) {

			TextBlockHighScore = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName("TextBlock_HighScore"));
			TextBlockCurrentScore = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName("TextBlock_CurrentScore"));
			ButtonMenu = Cast<UButton>(GameOverWidget->GetWidgetFromName("Button_Menu"));

			if (ButtonMenu) {
				ButtonMenu->OnClicked.AddDynamic(this, &AEndlessFlightGameModeBase::ButtonMenuClicked);
			}
		}
	}

	if (MenuWidgetClass) {
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget) {
			MenuWidget->AddToViewport();

			ButtonPlay = Cast<UButton>(MenuWidget->GetWidgetFromName(TEXT("Button_Play")));
			ButtonQuit = Cast<UButton>(MenuWidget->GetWidgetFromName(TEXT("Button_Quit")));

			if (ButtonPlay)
				ButtonPlay->OnClicked.AddDynamic(this, &AEndlessFlightGameModeBase::ButtonPlayClicked);

			if (ButtonQuit)
				ButtonQuit->OnClicked.AddDynamic(this, &AEndlessFlightGameModeBase::ButtonQuitClicked);
		}
	}

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	
}


void AEndlessFlightGameModeBase::SpawnNextActor(FVector NextLocation)
{
	if (SpawningActorClass == nullptr) return;

	ScoreValue++;

	FActorSpawnParameters SpawnParameters;

	GetWorld()->SpawnActor<AActor>(SpawningActorClass, NextLocation + SpawningDistance, FRotator::ZeroRotator, SpawnParameters);

	if (TextBlockScore) {
		TextBlockScore->SetText(FText::FromString(FString::FromInt(ScoreValue)));

		if(ScoreSound)
			UGameplayStatics::PlaySoundAtLocation(this, ScoreSound, NextLocation);
	}
	
}


void AEndlessFlightGameModeBase::ButtonMenuClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Button Menu Clicked"));
	GetWorld()->GetFirstPlayerController()->RestartLevel();
	//UGameplayStatics::OpenLevel(this, FName("Map_01"));
}


void AEndlessFlightGameModeBase::GameOver()
{
	if (bIsGameOver) return;
	LoadScore();


	if(ScoreValue > HighScoreValue) {
		SaveScore();
		HighScoreValue = ScoreValue;
	}



	if (GameOverWidget) {
		GameOverWidget->AddToViewport();
		if (TextBlockHighScore)
			TextBlockHighScore->SetText(FText::FromString("High Score : " + FString::FromInt(HighScoreValue)));

		if (TextBlockCurrentScore)
			TextBlockCurrentScore->SetText(FText::FromString("Current Score : " + FString::FromInt(ScoreValue)));

	}
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	CurrentWidget->RemoveFromViewport();

	if (GameOverSound)
		UGameplayStatics::PlaySound2D(this, GameOverSound);

	bIsGameOver = true;
}


void AEndlessFlightGameModeBase::ButtonPlayClicked()
{

	if (MenuWidget && MenuWidget->IsInViewport()) {

		ABasePawn* BaseP = Cast<ABasePawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (BaseP) {

			BaseP->bIsGameStarted = true;
			MenuWidget->RemoveFromViewport();
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		}
	}
}


void AEndlessFlightGameModeBase::ButtonQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit);
}


void AEndlessFlightGameModeBase::SaveScore()
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	SaveGameInstance->HighScoreValue = ScoreValue;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Slot0"), 0);
}


void AEndlessFlightGameModeBase::LoadScore()
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot0"), 0));
	if(SaveGameInstance)
		HighScoreValue = SaveGameInstance->HighScoreValue;
}
