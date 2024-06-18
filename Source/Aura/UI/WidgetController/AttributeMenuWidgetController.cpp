// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (const auto& [Tag, Attribute] : AS->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
		FGameplayAttribute Attr = Attribute.Execute();
		Info.AttributeValue = Attr.GetNumericValue(AS);
		AttributeDelegate.Broadcast(Info);
	} 
}
