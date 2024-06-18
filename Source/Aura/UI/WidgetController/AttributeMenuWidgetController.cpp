// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	const auto& Tags = FAuraGameplayTags::Get();
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tags.Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeDelegate.Broadcast(Info);
}
