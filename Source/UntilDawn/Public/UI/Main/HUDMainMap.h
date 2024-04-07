
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDMainMap.generated.h"

class UWidgetWrestlingProgress;
class APlayerControllerMainMap;

UCLASS()
class UNTILDAWN_API AHUDMainMap : public AHUD
{
	GENERATED_BODY()
	
public:

	AHUDMainMap();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void InitializeHUD();

	UFUNCTION()
	void IncreasingProgressBar();

	void CheckWrestlingProgress(float deltaTime);

	virtual void Tick(float deltaTime) override;
	
	UFUNCTION()
	void StartWrestlingProgressBar();

	void EndWrestlingProgressBar();

private:

	UPROPERTY()
	TSubclassOf<UWidgetWrestlingProgress> WidgetWrestlingProgressClass;

	UPROPERTY()
	UWidgetWrestlingProgress* WrestlingProgressWidget;

	TWeakObjectPtr<APlayerControllerMainMap> playerController;

	bool bStartWrestling;

};
