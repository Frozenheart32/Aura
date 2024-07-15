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

	AuraASC->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
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
	});
}
