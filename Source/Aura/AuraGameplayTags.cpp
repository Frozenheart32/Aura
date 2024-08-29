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

	/*
	 * Resistances
	 */
	GameplayTags.Attributes_Resistance_Fire = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Resistance.Fire"},
		FString{"Amount of Fire resistance"});

	GameplayTags.Attributes_Resistance_Lightning = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Resistance.Lightning"},
		FString{"Amount of Lightning resistance"});

	GameplayTags.Attributes_Resistance_Arcane = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Resistance.Arcane"},
		FString{"Amount of Arcane resistance"});

	GameplayTags.Attributes_Resistance_Physical = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Resistance.Physical"},
		FString{"Amount of Physical resistance"});

	/*
	 * Meta 
	 */

	GameplayTags.Attributes_Meta_IncomingXP = TagManager.AddNativeGameplayTag(
		FName{"Attributes.Meta.IncomingXP"},
		FString{"Incoming meta XP attribute"});

	/*
	 * Input Tags
	 */

	GameplayTags.InputTag_LMB = TagManager.AddNativeGameplayTag(
		FName{"InputTag.LMB"},
		FString{"Input tag for Left mouse button"});

	GameplayTags.InputTag_RMB = TagManager.AddNativeGameplayTag(
		FName{"InputTag.RMB"},
		FString{"Input tag for Right mouse button"});

	GameplayTags.InputTag_1 = TagManager.AddNativeGameplayTag(
		FName{"InputTag.1"},
		FString{"Input tag for 1 key button"});

	GameplayTags.InputTag_2 = TagManager.AddNativeGameplayTag(
		FName{"InputTag.2"},
		FString{"Input tag for 2 key button"});

	GameplayTags.InputTag_3 = TagManager.AddNativeGameplayTag(
		FName{"InputTag.3"},
		FString{"Input tag for 3 key button"});

	GameplayTags.InputTag_4 = TagManager.AddNativeGameplayTag(
		FName{"InputTag.4"},
		FString{"Input tag for 4 key button"});

	GameplayTags.InputTag_Passive_1 = TagManager.AddNativeGameplayTag(
		FName{"InputTag.Passive.1"},
		FString{"Input tag for Passive ability #1"});

	GameplayTags.InputTag_Passive_2 = TagManager.AddNativeGameplayTag(
		FName{"InputTag.Passive.2"},
		FString{"Input tag for Passive ability #2"});


	/*
	 * Damage
	 */
	GameplayTags.Damage = TagManager.AddNativeGameplayTag(
		FName{"Damage"},
		FString{"Damage"});

	GameplayTags.Damage_Fire = TagManager.AddNativeGameplayTag(
		FName{"Damage.Fire"},
		FString{"Fire Damage Type"});

	GameplayTags.Damage_Lightning = TagManager.AddNativeGameplayTag(
		FName{"Damage.Lightning"},
		FString{"Lightning Damage Type"});

	GameplayTags.Damage_Arcane = TagManager.AddNativeGameplayTag(
		FName{"Damage.Arcane"},
		FString{"Arcane Damage Type"});

	GameplayTags.Damage_Physical = TagManager.AddNativeGameplayTag(
		FName{"Damage.Physical"},
		FString{"Physical Damage Type"});


	//Bind damage types and resistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

	/*
	 * Debuffs
	 */
	GameplayTags.Debuff_Burn = TagManager.AddNativeGameplayTag(
		FName{"Debuff.Burn"},
		FString{"Burn Debuff Tag"});

	GameplayTags.Debuff_Stun = TagManager.AddNativeGameplayTag(
		FName{"Debuff.Stun"},
		FString{"Stun Debuff Tag"});

	GameplayTags.Debuff_Arcane = TagManager.AddNativeGameplayTag(
		FName{"Debuff.Arcane"},
		FString{"Arcane Debuff Tag"});

	GameplayTags.Debuff_Physical = TagManager.AddNativeGameplayTag(
		FName{"Debuff.Physical"},
		FString{"Physical Debuff Tag"});


	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Debuff_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);

	GameplayTags.Debuff_Chance = TagManager.AddNativeGameplayTag(
		FName{"Debuff.Chance"},
		FString{"Debuff Chance Tag"});

	GameplayTags.Debuff_Damage = TagManager.AddNativeGameplayTag(
		FName{"Debuff.Damage"},
		FString{"Debuff Damage Tag"});

	GameplayTags.Debuff_Duration = TagManager.AddNativeGameplayTag(
		FName{"Debuff.Duration"},
		FString{"Debuff Duration Tag"});

	GameplayTags.Debuff_Frequency = TagManager.AddNativeGameplayTag(
		FName{"Debuff.Frequency"},
		FString{"Debuff Frequency Tag"});
	
	GameplayTags.Abilities_None = TagManager.AddNativeGameplayTag(
		FName{"Abilities.None"},
		FString{"No Ability Tag - like the nullptr for Ability Tags"});

	
	GameplayTags.Abilities_Attack = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Attack"},
		FString{"Attack Ability Tag"});

	GameplayTags.Abilities_Summon = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Summon"},
		FString{"Summon Ability Tag"});


	GameplayTags.Abilities_HitReact = TagManager.AddNativeGameplayTag(
		FName{"Abilities.HitReact"},
		FString{"HitReact Ability Tag"});

	
	GameplayTags.Abilities_Type_None = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Type.None"},
		FString{"None Type Ability Tag"});

	GameplayTags.Abilities_Type_Offensive = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Type.Offensive"},
		FString{"Offensive Type Ability Tag"});

	GameplayTags.Abilities_Type_Passive = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Type.Passive"},
		FString{"Passive Type Ability Tag"});
	
	GameplayTags.Abilities_Status_Eligible = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Status.Eligible"},
		FString{"Eligible Status Tag"});

	GameplayTags.Abilities_Status_Equipped = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Status.Equipped"},
		FString{"Equipped Status Tag"});

	GameplayTags.Abilities_Status_Locked = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Status.Locked"},
		FString{"Locked Status Tag"});

	GameplayTags.Abilities_Status_Unlocked = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Status.Unlocked"},
		FString{"Unlocked Status Tag"});
	

	/*
	 * Offensive Spells
	 */
	GameplayTags.Abilities_Fire_Firebolt = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Fire.Firebolt"},
		FString{"Firebolt Ability Tag"});

	GameplayTags.Abilities_Fire_FireBlast = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Fire.FireBlast"},
		FString{"FireBlast Ability Tag"});


	GameplayTags.Abilities_Lightning_Electrocute = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Lightning.Electrocute"},
		FString{"Electrocute Ability Tag"});


	GameplayTags.Abilities_Arcane_ArcaneShards = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Arcane.ArcaneShards"},
		FString{"ArcaneShards Ability Tag"});


	/*
	 * Passive spells
	 */

	GameplayTags.Abilities_Passive_HaloOfProtection = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Passive.HaloOfProtection"},
		FString{"Halo Of Protection Ability Tag"});

	GameplayTags.Abilities_Passive_LifeSiphon = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Passive.LifeSiphon"},
		FString{"Life Siphon Ability Tag"});

	GameplayTags.Abilities_Passive_ManaSiphon = TagManager.AddNativeGameplayTag(
		FName{"Abilities.Passive.ManaSiphon"},
		FString{"Mana Siphon Ability Tag"});


	/*
	 * Cooldown 
	 */

	GameplayTags.Cooldown_Fire_Firebolt = TagManager.AddNativeGameplayTag(
		FName{"Cooldown.Fire.Firebolt"},
		FString{"Firebolt Cooldown Tag"});

	GameplayTags.Cooldown_Fire_FireBlast = TagManager.AddNativeGameplayTag(
		FName{"Cooldown.Fire.FireBlast"},
		FString{"FireBlast Cooldown Tag"});

	GameplayTags.Cooldown_Lightning_Electrocute = TagManager.AddNativeGameplayTag(
		FName{"Cooldown.Lightning.Electrocute"},
		FString{"Electrocute Cooldown Tag"});

	GameplayTags.Cooldown_Arcane_ArcaneShards = TagManager.AddNativeGameplayTag(
		FName{"Cooldown.Arcane.ArcaneShards"},
		FString{"Arcane Shards Cooldown Tag"});
	/*
	 * Combat sockets
	 */

	GameplayTags.CombatSocket_Weapon = TagManager.AddNativeGameplayTag(
		FName{"CombatSocket.Weapon"},
		FString{"Weapon"});

	GameplayTags.CombatSocket_RightHand = TagManager.AddNativeGameplayTag(
		FName{"CombatSocket.RightHand"},
		FString{"RightHand"});

	GameplayTags.CombatSocket_LeftHand = TagManager.AddNativeGameplayTag(
		FName{"CombatSocket.LeftHand"},
		FString{"LeftHand"});

	GameplayTags.CombatSocket_Tail = TagManager.AddNativeGameplayTag(
		FName{"CombatSocket.Tail"},
		FString{"Tail"});

	/*
	 * Montage tags
	 */

	GameplayTags.Montage_Attack_1 = TagManager.AddNativeGameplayTag(
		FName{"Montage.Attack.1"},
		FString{"Attack 1"});

	GameplayTags.Montage_Attack_2 = TagManager.AddNativeGameplayTag(
		FName{"Montage.Attack.2"},
		FString{"Attack 2"});

	GameplayTags.Montage_Attack_3 = TagManager.AddNativeGameplayTag(
		FName{"Montage.Attack.3"},
		FString{"Attack 3"});

	GameplayTags.Montage_Attack_4 = TagManager.AddNativeGameplayTag(
		FName{"Montage.Attack.4"},
		FString{"Attack 4"});
	
	/*
	 * Effects
	 */

	GameplayTags.Effects_HitReact = TagManager.AddNativeGameplayTag(
		FName{"Effects.HitReact"},
		FString{"Tag granted when Hit Reacting"});


	/*
	 * Player Tags
	 */
	
	GameplayTags.Player_Block_CursorTrace = TagManager.AddNativeGameplayTag(
		FName{"Player.Block.CursorTrace"},
		FString{"Block tracing under the cursor"});

	GameplayTags.Player_Block_InputHeld = TagManager.AddNativeGameplayTag(
		FName{"Player.Block.InputHeld"},
		FString{"Block Input Held callback for input"});

	GameplayTags.Player_Block_InputPressed = TagManager.AddNativeGameplayTag(
		FName{"Player.Block.InputPressed"},
		FString{"Block Input Pressed callback for input"});

	GameplayTags.Player_Block_InputReleased = TagManager.AddNativeGameplayTag(
		FName{"Player.Block.InputReleased"},
		FString{"Block Input Released callback for input"});
}
