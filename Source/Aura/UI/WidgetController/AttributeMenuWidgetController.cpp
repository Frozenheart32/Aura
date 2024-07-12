// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = GetOwningAttributeSet();
	for (const auto& [Tag, Attribute] : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute.Execute())
			.AddLambda([this, Tag, Attribute](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag, Attribute.Execute());
			});
	}

	const auto AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnAttributePointsChanged.AddLambda(
		[this](int32 Points)
	{
		AttributePointsChangedDelegate.Broadcast(Points);
	});

	AuraPlayerState->OnSpellPointsChanged.AddLambda(
		[this](int32 Points)
	{
		SpellPointsChangedDelegate.Broadcast(Points);
	});
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	const UAuraAttributeSet* AS = GetOwningAttributeSet();
	const auto AuraPlayerState = GetOwningPlayerState();

	for (const auto& [Tag, Attribute] : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Tag, Attribute.Execute());
	}

	AttributePointsChangedDelegate.Broadcast(AuraPlayerState->GetAttributePoints());
	SpellPointsChangedDelegate.Broadcast(AuraPlayerState->GetSpellPoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	const auto AuraASC = GetOwningASC();
	AuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet.Get());
	AttributeDelegate.Broadcast(Info);
}
