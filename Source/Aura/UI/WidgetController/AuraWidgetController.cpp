// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	const auto AuraASC = GetOwningASC();
	if(!AuraASC || !GetOwningASC()->bStartupAbilitiesGiven) return;

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

AAuraPlayerController* UAuraWidgetController::GetOwningPlayerController() const
{
	return CastChecked<AAuraPlayerController>(PlayerController.Get());
}

AAuraPlayerState* UAuraWidgetController::GetOwningPlayerState() const
{
	return CastChecked<AAuraPlayerState>(PlayerState.Get());
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetOwningASC() const
{
	return CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent.Get());
}

UAuraAttributeSet* UAuraWidgetController::GetOwningAttributeSet() const
{
	return CastChecked<UAuraAttributeSet>(AttributeSet.Get());
}
