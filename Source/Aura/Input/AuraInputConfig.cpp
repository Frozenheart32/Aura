// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& Input, bool bLogNotFound) const
{
	for (const auto& Action : AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag.MatchesTagExact(Input))
		{
			return Action.InputAction;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for Input [%s], on InputConfig [%s]"), *Input.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const TArray<FAuraInputAction>& UAuraInputConfig::GetAbilityInputActions() const
{
	return AbilityInputActions;
}
