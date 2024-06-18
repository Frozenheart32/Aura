// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	auto& TagManager = UGameplayTagsManager::Get();

	/*
	 * Primary Attributes Tags
	 */
	
	GameplayTags.Attributes_Primary_Strength = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Primary.Strength"},
		FString{"Increases physical damage"});

	GameplayTags.Attributes_Primary_Intelligence = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Primary.Intelligence"},
		FString{"Increases magical damage"});

	GameplayTags.Attributes_Primary_Resilience = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Primary.Resilience"},
		FString{"Increases Armor and Armor Penetration"});

	GameplayTags.Attributes_Primary_Vigor = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Primary.Vigor"},
		FString{"Increases Health"});
	

	/*
	 * Secondary Attributes Tags
	 */
	
	GameplayTags.Attributes_Secondary_Armor = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.Armor"},
		FString{"Reduces damage taken, improves block chance."});

	GameplayTags.Attributes_Secondary_ArmorPenetration = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.ArmorPenetration"},
		FString{"Ignores Percentage of enemy Armor, increases Critical Hit Chance"});

	GameplayTags.Attributes_Secondary_BlockChance = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.BlockChance"},
		FString{"Chance to cut incoming damage in half"});

	GameplayTags.Attributes_Secondary_CriticalHitChance = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.CriticalHitChance"},
		FString{"Chance to double damage plus critical hit bonus"});

	GameplayTags.Attributes_Secondary_CriticalHitDamage = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.CriticalHitDamage"},
		FString{"Bonus damage added when a critical hit is scored"});

	GameplayTags.Attributes_Secondary_CriticalHitResistance = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.CriticalHitResistance"},
		FString{"Reduces Critical Hit Chance of attacking enemies"});

	GameplayTags.Attributes_Secondary_HealthRegeneration = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.HealthRegeneration"},
		FString{"Amount of Health regenerated every 1 second"});

	GameplayTags.Attributes_Secondary_ManaRegeneration = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.ManaRegeneration"},
		FString{"Amount of Mana regenerated every 1 second"});

	GameplayTags.Attributes_Secondary_MaxHealth = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.MaxHealth"},
		FString{"Maximum amount of Health obtainable"});

	GameplayTags.Attributes_Secondary_MaxMana = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Secondary.MaxMana"},
		FString{"Maximum amount of Mana obtainable"});
}
