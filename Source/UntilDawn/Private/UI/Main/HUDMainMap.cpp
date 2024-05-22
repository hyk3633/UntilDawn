
#include "UI/Main/HUDMainMap.h"
#include "UI/Main/WidgetMainInterface.h"
#include "UI/Main/WidgetWrestlingProgress.h"
#include "../UntilDawn.h"
#include "Player/Main/PlayerControllerMainMap.h"

AHUDMainMap::AHUDMainMap()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UWidgetMainInterface> WBP_MainInterface
	(TEXT("WidgetBlueprint'/Game/_Assets/WidgetBlueprints/Main/WBP_MainInterface.WBP_MainInterface_C'"));
	if (WBP_MainInterface.Succeeded()) widgetMainInterfaceClass = WBP_MainInterface.Class;

	static ConstructorHelpers::FClassFinder<UWidgetWrestlingProgress> WBP_WrestlingProgress
	(TEXT("WidgetBlueprint'/Game/_Assets/WidgetBlueprints/Wrestling/WBP_Wrestling.WBP_Wrestling_C'"));
	if (WBP_WrestlingProgress.Succeeded()) widgetWrestlingProgressClass = WBP_WrestlingProgress.Class;
}

void AHUDMainMap::BeginPlay()
{
	Super::BeginPlay();

}

void AHUDMainMap::StartHUD()
{
	playerController = Cast<APlayerControllerMainMap>(GetOwningPlayerController());
	check(playerController.Get());
	playerController->DPlayerDead.BindUFunction(this, FName("InitializeHUD"));
	playerController->DEKeyPressed.BindUFunction(this, FName("IncreasingProgressBar"));
	playerController->DIKeyPressed.BindUFunction(this, FName("ToggleInventoryUI"));
	playerController->DWrestlingStart.BindUFunction(this, FName("StartWrestlingProgressBar"));

	if (widgetMainInterfaceClass)
	{
		mainInterfaceWidget = CreateWidget<UWidgetMainInterface>(GetOwningPlayerController(), widgetMainInterfaceClass);
		mainInterfaceWidget->InitializeWidget();
		mainInterfaceWidget->AddToViewport();
	}

	if (widgetWrestlingProgressClass)
	{
		wrestlingProgressWidget = CreateWidget<UWidgetWrestlingProgress>(GetOwningPlayerController(), widgetWrestlingProgressClass);
		wrestlingProgressWidget->SetVisibility(ESlateVisibility::Hidden);
		wrestlingProgressWidget->AddToViewport();
	}
}

void AHUDMainMap::InitializeHUD()
{
	EndWrestlingProgressBar();
}

void AHUDMainMap::ToggleInventoryUI()
{
	mainInterfaceWidget->ToggleInventoryUI();
}

void AHUDMainMap::IncreasingProgressBar()
{
	if (wrestlingProgressWidget->IncreasingProgressBar())
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
	if (bStartWrestling)
	{
		check(wrestlingProgressWidget);
		if (wrestlingProgressWidget->ReducingProgressBar(deltaTime))
		{
			playerController->FailedToBlocking();
			EndWrestlingProgressBar();
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
	wrestlingProgressWidget->SetVisibility(ESlateVisibility::Visible);
	wrestlingProgressWidget->PlayHighlightTextAnimation();
}

void AHUDMainMap::EndWrestlingProgressBar()
{
	bStartWrestling = false;
	wrestlingProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	wrestlingProgressWidget->ResetProgressBar();
}
