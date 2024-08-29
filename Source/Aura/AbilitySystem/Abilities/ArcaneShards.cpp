// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 CurrentLevel) const
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(CurrentLevel);
	const float ManaCost = GetManaCost(CurrentLevel);
	const float Cooldown = GetCooldown(CurrentLevel);
	if(CurrentLevel == 1)
	{
		
		return FString::Printf(
			TEXT("<Title>ARCANE SHARDS</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Summon shards of arcane energy, "
					"causing arcane damage of </>"
					"<Damage>%d</><Default> at the shard origin</>\n\n"),
			CurrentLevel,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}	
	else
	{
		return FString::Printf(
			TEXT("<Title>ARCANE SHARDS</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Summon %d shards of arcane energy, "
					"causing arcane damage of </>"
					"<Damage>%d</><Default> at the shard origin</>\n\n"),
			CurrentLevel,
			ManaCost,
			Cooldown,
			FMath::Min(MaxNumShards - 1, CurrentLevel),
			ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 CurrentLevel) const
{
	const int32 NextLevel = CurrentLevel + 1;
	const int32 ScaledDamage = Damage.GetValueAtLevel(NextLevel);
	const float ManaCost = GetManaCost(NextLevel);
	const float Cooldown = GetCooldown(NextLevel);
	
	return FString::Printf(
			TEXT("<Title>NEXT LEVEL</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Summon %d shards of arcane energy, "
					"causing arcane damage of </>"
					"<Damage>%d</><Default> at the shard origin</>\n\n"),
			NextLevel,
			ManaCost,
			Cooldown,
			FMath::Min(MaxNumShards - 1, NextLevel),
			ScaledDamage);
}

int32 UArcaneShards::GetMaxShardsCountAtLevel() const
{
	return FMath::Min(MaxNumShards, GetAbilityLevel());
}
