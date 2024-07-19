// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 CurrentLevel) const
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), 
		L"Default Ability Name - LoreIpsum", CurrentLevel);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 CurrentLevel) const
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), 
		L"NextLevel: - LoreIpsum blablabla", CurrentLevel + 1);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), 
		L"Spell Locked Until Level: ", Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if(const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for(const FGameplayModifierInfo& Mod : CostEffect->Modifiers)
		{
			if(Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}

	if(ManaCost < 0.f)
	{
		ManaCost *= -1.f;
	}

	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	float CooldownDuration = 0.f;
	if(const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CooldownDuration);
	}

	return CooldownDuration;
}
