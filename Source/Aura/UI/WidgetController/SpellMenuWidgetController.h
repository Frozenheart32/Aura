// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);

USTRUCT(BlueprintType)
struct FSelectedAbility
{
	GENERATED_BODY()

	FSelectedAbility()
	{
		const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get(); 
		AbilityTag = AuraTags.Abilities_None;
		StatusTag = AuraTags.Abilities_Status_Locked;
	}

	FGameplayTag AbilityTag = FGameplayTag{};
	FGameplayTag StatusTag = FGameplayTag{};
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangeSignature SpellPointsChanged;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature OnSpellSelectedDelegate;

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

private:

	static void ShouldEnableButtons(bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton, const FGameplayTag& AbilityStatus, int32 SpellPoints);

	FSelectedAbility SelectedAbility;
	int32 CurrentSpellPoints = 0;
};
