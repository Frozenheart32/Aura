// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

APlayerController* UAuraWidgetController::GetOwningPlayerController() const
{
	return PlayerController.Get();
}

APlayerState* UAuraWidgetController::GetOwningPlayerState() const
{
	return PlayerState.Get();
}

UAbilitySystemComponent* UAuraWidgetController::GetOwningASC() const
{
	return AbilitySystemComponent.Get();
}

UAttributeSet* UAuraWidgetController::GetOwningAttributeSet() const
{
	return AttributeSet.Get();
}
