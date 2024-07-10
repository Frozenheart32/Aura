// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const auto AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnXPChanged.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	AuraPlayerState->OnLevelChanged.AddUObject(this, &UOverlayWidgetController::OnLevelChanged);

	
	const auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnHealthChanged.Broadcast(Data.NewValue);
	                      });
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	
	const auto AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if(AuraASC->bStartupAbilitiesGiven)
	{
		OnInitializeStartupAbilities(AuraASC);
	}
	else
	{
		AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
	}
	
	AuraASC->EffectAssetTags
	       .AddLambda([this](const FGameplayTagContainer& AssetTags)
	       {
		       const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("Message");
		       for(const FGameplayTag& Tag : AssetTags)
		       {
			       if(!Tag.MatchesTag(MessageTag)) continue;
				
			       const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			       MessageWidgetRowDelegate.Broadcast(*Row);
		       }
	       });
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if(Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / DeltaLevelRequirement;

		OnXPPercentChanged.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnLevelChanged(int32 NewLevel) const
{
	OnPlayerLevelChanged.Broadcast(NewLevel);
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
{
	//TODO: Get information about all given abilities, took up their ability info and broadcast it to widgets
	if(!AuraASC->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate{};
	BroadcastDelegate.BindLambda([this, AuraASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo AuraAbilityInfo = AbilityInfo->FindAbilityInfoByTag(
			AuraASC->GetAbilityTagFromSpec(AbilitySpec));
		AuraAbilityInfo.InputTag = AuraASC->GetInputTagForSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(AuraAbilityInfo);
	});

	AuraASC->ForEachAbility(BroadcastDelegate);
}
