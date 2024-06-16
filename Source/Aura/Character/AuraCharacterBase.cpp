// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacterBase.h"

#include "AbilitySystemComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName{"WeaponHandSocket"});
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitializePrimaryAttributes() const
{
	check(AbilitySystemComponent.IsValid())
	check(DefaultPrimaryAttributes);

	const FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultPrimaryAttributes, 1, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), AbilitySystemComponent.Get());
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet.Get();
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
	
}

