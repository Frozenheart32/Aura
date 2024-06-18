// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (const auto& [Tag, Attribute] : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute.Execute())
			.AddLambda([this, Tag, Attribute](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag, Attribute.Execute());
			});
	} 
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (const auto& [Tag, Attribute] : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Tag, Attribute.Execute());
	} 
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet.Get());
	AttributeDelegate.Broadcast(Info);
}
