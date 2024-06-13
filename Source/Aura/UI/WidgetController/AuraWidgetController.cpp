// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

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
