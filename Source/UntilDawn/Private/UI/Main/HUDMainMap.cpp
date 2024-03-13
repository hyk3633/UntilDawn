
#include "UI/Main/HUDMainMap.h"
#include "UI/Main/WidgetWrestlingProgress.h"
#include "../UntilDawn.h"
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

	if (WidgetWrestlingProgressClass)
	{
		WrestlingProgressWidget = CreateWidget<UWidgetWrestlingProgress>(GetOwningPlayerController(), WidgetWrestlingProgressClass);
		WrestlingProgressWidget->SetVisibility(ESlateVisibility::Hidden);
		WrestlingProgressWidget->AddToViewport();
	}
}

void AHUDMainMap::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (bStartWrestling && WrestlingProgressWidget)
	{
		if (WrestlingProgressWidget->ReducingProgressBar(deltaTime))
		{
			DFailedToResist.ExecuteIfBound();
		}
	}
}

bool AHUDMainMap::IncreasingProgressBar()
{
	return WrestlingProgressWidget->IncreasingProgressBar();
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
