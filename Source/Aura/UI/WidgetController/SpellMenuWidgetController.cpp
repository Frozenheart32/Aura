// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	const auto AuraPS = GetOwningPlayerState();
	check(AuraPS);
	SpellPointsChanged.Broadcast(AuraPS->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	const auto AuraASC = GetOwningASC();
	check(AuraASC);

	AuraASC->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if(SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.StatusTag = StatusTag;

			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;

			ShouldEnableButtons(bEnableSpendPoints, bEnableEquip, StatusTag, CurrentSpellPoints);
			FString Description, NextLevelDescription;
			GetOwningASC()->GetDescriptionsByAbilityTag(Description, NextLevelDescription, AbilityTag);
			OnSpellSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, MoveTemp(Description), MoveTemp(NextLevelDescription));
		}
		
		if(AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	const auto AuraPS = GetOwningPlayerState();
	check(AuraPS);

	AuraPS->OnSpellPointsChanged.AddLambda([this](int32 NewSpellPointCount)
	{
		SpellPointsChanged.Broadcast(NewSpellPointCount);
		CurrentSpellPoints = NewSpellPointCount;

		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;

		FString Description, NextLevelDescription;
		GetOwningASC()->GetDescriptionsByAbilityTag(Description, NextLevelDescription, SelectedAbility.AbilityTag);
		OnSpellSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, MoveTemp(Description), MoveTemp(NextLevelDescription));
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if(bWaitingForEquipSelection)
	{
		const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(AbilityTag).TypeTag;
		OnStopWaitForEquipDelegate.Broadcast(AbilityType);
		bWaitingForEquipSelection = false;
	}

	const auto& AuraTags = FAuraGameplayTags::Get();
	const auto AuraPS = GetOwningPlayerState();
	check(AuraPS);

	const int32 SpellPoints = AuraPS->GetSpellPoints();

	const auto AuraASC = GetOwningASC();
	check(AuraASC);

	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(AuraTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpecPtr = AuraASC->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpecPtr != nullptr;
	
	if(!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = AuraTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = AuraASC->GetStatusFromSpec(*AbilitySpecPtr);
	}

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;
	
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;

	ShouldEnableButtons(bEnableSpendPoints, bEnableEquip, AbilityStatus, SpellPoints);
	FString Description, NextLevelDescription;
	GetOwningASC()->GetDescriptionsByAbilityTag(Description, NextLevelDescription, AbilityTag);
	OnSpellSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, MoveTemp(Description), MoveTemp(NextLevelDescription));
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if(GetOwningASC())
	{
		GetOwningASC()->SpendSpellPoint_OnServer(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if(bWaitingForEquipSelection)
	{
		check(AbilityInfo);
		const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).TypeTag;
		OnStopWaitForEquipDelegate.Broadcast(AbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const auto& AuraTags = FAuraGameplayTags::Get();
	
	SelectedAbility.AbilityTag = AuraTags.Abilities_None;
	SelectedAbility.StatusTag = AuraTags.Abilities_Status_Locked;

	OnSpellSelectedDelegate.Broadcast(false, false, FString{}, FString{});
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	check(AbilityInfo);
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).TypeTag;

	OnWaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
}

void USpellMenuWidgetController::ShouldEnableButtons(bool& bShouldEnableSpellPointsButton,
                                                     bool& bShouldEnableEquipButton, const FGameplayTag& AbilityStatus, int32 SpellPoints)
{
	const auto& AuraTags = FAuraGameplayTags::Get();
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	
	if(AbilityStatus.MatchesTagExact(AuraTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpellPointsButton = SpellPoints > 0;
	}
	else if(AbilityStatus.MatchesTagExact(AuraTags.Abilities_Status_Eligible))
	{
		bShouldEnableSpellPointsButton = SpellPoints > 0;
	}
	else if(AbilityStatus.MatchesTagExact(AuraTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpellPointsButton = SpellPoints > 0;
	}
}
