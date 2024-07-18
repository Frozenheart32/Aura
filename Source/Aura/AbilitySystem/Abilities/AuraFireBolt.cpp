// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level) const
{
	const auto& AuraTags = FAuraGameplayTags::Get();
	const int32 Damage = GetDamageByType(Level, AuraTags.Damage_Fire);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	if(Level == 1)
	{
		
		return FString::Printf(
			TEXT("<Title>FIRE BOLT</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched a bolt of fire, "
					"exploding on impact and dealing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			Level,
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
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(NumProjectiles, Level),
			Damage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level) const
{
	++Level;
	const auto& AuraTags = FAuraGameplayTags::Get();
	const int32 Damage = GetDamageByType(Level, AuraTags.Damage_Fire);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(
			TEXT("<Title>NEXT LEVEL</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched %d bolts of fire, "
					"exploding on impact and dealing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(NumProjectiles, Level),
			Damage);
}