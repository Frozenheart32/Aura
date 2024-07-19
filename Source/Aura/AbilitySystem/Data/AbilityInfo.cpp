// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const auto& Info : AbilityInformation)
	{
		if(Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Info;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info by AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAbilityInfo{};
}

const TArray<FAuraAbilityInfo>& UAbilityInfo::GetAbilityInformation() const
{
	return AbilityInformation;
}
