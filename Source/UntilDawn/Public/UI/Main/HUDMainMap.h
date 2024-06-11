
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDMainMap.generated.h"

class UWidgetMainInterface;
class UWidgetWrestlingProgress;
class APlayerControllerMainMap;

UCLASS()
class UNTILDAWN_API AHUDMainMap : public AHUD
{
	GENERATED_BODY()
	
public:

	AHUDMainMap();

	void StartHUD();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void InitializeHUD();

	UFUNCTION()
	void ToggleInventoryUI();

	UFUNCTION()
	void IncreasingProgressBar();

	void CheckWrestlingProgress(float deltaTime);

	virtual void Tick(float deltaTime) override;
	
	UFUNCTION()
	void StartWrestlingProgressBar();

	UFUNCTION()
	void EndWrestlingProgressBar();

private:

	UPROPERTY()
	TSubclassOf<UWidgetMainInterface> widgetMainInterfaceClass;

	UPROPERTY()
	UWidgetMainInterface* mainInterfaceWidget;

	UPROPERTY()
	TSubclassOf<UWidgetWrestlingProgress> widgetWrestlingProgressClass;

	UPROPERTY()
	UWidgetWrestlingProgress* wrestlingProgressWidget;

	TWeakObjectPtr<APlayerControllerMainMap> playerController;

	bool bStartWrestling;

};
