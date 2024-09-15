// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class ULoadScreenViewModel;
class ULoadScreenWidget;
class UUserWidget;

/**
 * 
 */
UCLASS()
class AURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenViewModel> LoadScreenViewModel;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadScreenViewModel> LoadScreenViewModelClass;

	virtual void BeginPlay() override;
};
