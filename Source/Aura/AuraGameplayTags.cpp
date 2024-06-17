// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	auto& TagManager = UGameplayTagsManager::Get();

	TagManager.AddNativeGameplayTag(FName{"Attributes.Secondary.Armor"},
		FString{" Reduces damage taken, improves block chance."});
}
