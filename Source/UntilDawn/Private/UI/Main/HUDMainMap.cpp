
#include "UI/Main/HUDMainMap.h"
#include "UI/Main/WidgetWrestlingProgress.h"
#include "../UntilDawn.h"
#include "Player/Main/PlayerControllerMainMap.h"

AHUDMainMap::AHUDMainMap()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UWidgetWrestlingProgress> WBP_WrestlingProgress
	(TEXT("WidgetBlueprint'/Game/_Assets/WidgetBlueprints/Wrestling/WBP_Wrestling.WBP_Wrestling_C'"));
	if (WBP_WrestlingProgress.Succeeded()) WidgetWrestlingProgressClass = WBP_WrestlingProgress.Class;
}

void AHUDMainMap::BeginPlay()
{
	Super::BeginPlay();

	playerController = Cast<APlayerControllerMainMap>(GetOwningPlayerController());
	check(playerController.Get());
	playerController->DPlayerDead.BindUFunction(this, FName("InitializeHUD"));
	playerController->DEKeyPressed.BindUFunction(this, FName("IncreasingProgressBar"));
	playerController->DWrestlingStart.BindUFunction(this, FName("StartWrestlingProgressBar"));

	if (WidgetWrestlingProgressClass)
	{
		WrestlingProgressWidget = CreateWidget<UWidgetWrestlingProgress>(GetOwningPlayerController(), WidgetWrestlingProgressClass);
		WrestlingProgressWidget->SetVisibility(ESlateVisibility::Hidden);
		WrestlingProgressWidget->AddToViewport();
	}
}

void AHUDMainMap::InitializeHUD()
{
	EndWrestlingProgressBar();
}

void AHUDMainMap::IncreasingProgressBar()
{
	if (WrestlingProgressWidget->IncreasingProgressBar())
	{
		if (playerController.IsValid())
		{
			playerController->SuccessToBlocking();
		}
		EndWrestlingProgressBar();
	}
}

void AHUDMainMap::CheckWrestlingProgress(float deltaTime)
{
	if (bStartWrestling && WrestlingProgressWidget)
	{
		if (WrestlingProgressWidget->ReducingProgressBar(deltaTime))
		{
			playerController->FailedToBlocking();
		}
	}
}

void AHUDMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	CheckWrestlingProgress(deltaTime);
}

void AHUDMainMap::StartWrestlingProgressBar()
{
	bStartWrestling = true;
	WrestlingProgressWidget->SetVisibility(ESlateVisibility::Visible);
	WrestlingProgressWidget->PlayHighlightTextAnimation();
}

void AHUDMainMap::EndWrestlingProgressBar()
{
	bStartWrestling = false;
	WrestlingProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	WrestlingProgressWidget->ResetProgressBar();
}
