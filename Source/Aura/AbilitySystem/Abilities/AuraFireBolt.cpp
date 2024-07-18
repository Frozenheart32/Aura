// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 CurrentLevel) const
{
	const auto& AuraTags = FAuraGameplayTags::Get();
	const int32 Damage = GetDamageByType(CurrentLevel, AuraTags.Damage_Fire);
	const float ManaCost = GetManaCost(CurrentLevel);
	const float Cooldown = GetCooldown(CurrentLevel);
	if(CurrentLevel == 1)
	{
		
		return FString::Printf(
			TEXT("<Title>FIRE BOLT</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched a bolt of fire, "
					"exploding on impact and dealing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			CurrentLevel,
			ManaCost,
			Cooldown,
			Damage);
	}	
	else
	{
		return FString::Printf(
			TEXT("<Title>FIRE BOLT</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched %d bolts of fire, "
					"exploding on impact and dealing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			CurrentLevel,
			ManaCost,
			Cooldown,
			FMath::Min(NumProjectiles, CurrentLevel),
			Damage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 CurrentLevel) const
{
	const int32 NextLevel = CurrentLevel + 1;
	const auto& AuraTags = FAuraGameplayTags::Get();
	const int32 Damage = GetDamageByType(NextLevel, AuraTags.Damage_Fire);
	const float ManaCost = GetManaCost(NextLevel);
	const float Cooldown = GetCooldown(NextLevel);
	return FString::Printf(
			TEXT("<Title>NEXT LEVEL</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched %d bolts of fire, "
					"exploding on impact and dealing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			NextLevel,
			ManaCost,
			Cooldown,
			FMath::Min(NumProjectiles, NextLevel),
			Damage);
}