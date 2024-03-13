
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDMainMap.generated.h"

DECLARE_DELEGATE(DelegateFailedToResist);

class UWidgetWrestlingProgress;

UCLASS()
class UNTILDAWN_API AHUDMainMap : public AHUD
{
	GENERATED_BODY()
	
public:

	AHUDMainMap();

	DelegateFailedToResist DFailedToResist;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float deltaTime) override;

	bool IncreasingProgressBar();

	void StartWrestlingProgressBar();
	void EndWrestlingProgressBar();

private:

	UPROPERTY()
	TSubclassOf<UWidgetWrestlingProgress> WidgetWrestlingProgressClass;

	UPROPERTY()
	UWidgetWrestlingProgress* WrestlingProgressWidget;

	bool bStartWrestling;

};
