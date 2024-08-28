// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

bool UAuraAbilitySystemLibrary::TryGetWidgetControllerParams(FWidgetControllerParams& OutParams, AAuraHUD*& OutAuraHUD, const UObject* WorldContextObject)
{
	if(!WorldContextObject) return false;
	
	if(const auto PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			OutAuraHUD = AuraHUD;

			AAuraPlayerState* PS = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			OutParams = FWidgetControllerParams{PlayerController, PS, ASC, AS};
			return true;
		}
	}

	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if(!WorldContextObject) return nullptr;

	AAuraHUD* AuraHUD = nullptr;
	FWidgetControllerParams Params;
	check(TryGetWidgetControllerParams(Params, AuraHUD, WorldContextObject))
	return AuraHUD->GetOverlayWidgetController(Params);
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if(!WorldContextObject) return nullptr;

	AAuraHUD* AuraHUD = nullptr;
	FWidgetControllerParams Params;
	check(TryGetWidgetControllerParams(Params, AuraHUD, WorldContextObject))
	return AuraHUD->GetAttributeMenuWidgetController(Params);
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	if(!WorldContextObject) return nullptr;

	AAuraHUD* AuraHUD = nullptr;
	FWidgetControllerParams Params;
	check(TryGetWidgetControllerParams(Params, AuraHUD, WorldContextObject))
	return AuraHUD->GetSpellMenuWidgetController(Params);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level)
{
	const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(!CharacterClassInfo|| !ASC) return;
	
	const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	const AActor* AvatarActor = ASC->GetAvatarActor();
	
	//Apply Primary Attributes
	FGameplayEffectContextHandle PrimaryEffectContext = ASC->MakeEffectContext();
	PrimaryEffectContext.AddSourceObject(AvatarActor);
	const auto PrimaryAttributesEffectSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesEffectSpecHandle.Data.Get());

	//Apply Secondary Attributes
	FGameplayEffectContextHandle SecondaryEffectContext = ASC->MakeEffectContext();
	SecondaryEffectContext.AddSourceObject(AvatarActor);
	const auto SecondaryAttributesEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level,SecondaryEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesEffectSpecHandle.Data.Get());

	//Apply Vital Attributes
	FGameplayEffectContextHandle VitalEffectContext = ASC->MakeEffectContext();
	VitalEffectContext.AddSourceObject(AvatarActor);
	const auto VitalAttributesEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesEffectSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(!CharacterClassInfo || !ASC) return;
	
	int32 Level = 1;
	if(ASC->GetAvatarActor()->Implements<UCombatInterface>())
	{
		Level = ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor());
	}

	for (const auto& AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		const auto AbilitySpec = FGameplayAbilitySpec{AbilityClass, Level};
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfoByClass = CharacterClassInfo->CharacterClassInformation[CharacterClass];
	for (const auto& AbilityClass : DefaultInfoByClass.StartupAbilities)
	{
		const auto AbilitySpec = FGameplayAbilitySpec{AbilityClass, Level};
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!AuraGameMode) return nullptr;

	return AuraGameMode->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!AuraGameMode) return nullptr;

	return AuraGameMode->AbilityInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}

	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}

	return false;
}

bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsSuccessfulDebuff();
	}

	return false;
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDamage();
	}

	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDuration();
	}

	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffFrequency();
	}

	return 0.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const auto DamageType = AuraEffectContext->GetDamageType();
		if(DamageType.IsValid())
		{
			return *DamageType.Get();
		}
	}

	return FGameplayTag{};
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeathImpulse();
	}

	return FVector{};
}

FVector UAuraAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetKnockbackForce();
	}

	return FVector{};
}

bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsRadialDamage();
	}

	return false;
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageInnerRadius();
	}

	return 0.f;
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOuterRadius();
	}

	return 0.f;
}

FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOrigin();
	}

	return FVector{};
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsSuccessfulDebuff)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,
	float InDebuffDuration)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,
	float InDebuffFrequency)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		return AuraEffectContext->SetDamageType(DamageType);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& NewDeathImpulse)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetDeathImpulse(NewDeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& NewKnockbackForce)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetKnockbackForce(NewKnockbackForce);
	}
}

void UAuraAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InRadialDamageInnerRadius)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetRadialDamageInnerRadius(InRadialDamageInnerRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InRadialDamageOuterRadius)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetRadialDamageOuterRadius(InRadialDamageOuterRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InRadialDamageOrigin)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetRadialDamageOrigin(InRadialDamageOrigin);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithRadius(const UObject* WorldContextObject,
                                                         TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore,
                                                         float Radius, const FVector& SphereOrigin)
{
	if(WorldContextObject == nullptr) return;

	FCollisionQueryParams SphereParams{};
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if(const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
			FCollisionObjectQueryParams{FCollisionObjectQueryParams::InitType::AllDynamicObjects},
			FCollisionShape::MakeSphere(Radius), SphereParams);

		for (const auto& OverlapResult : Overlaps)
		{
			if(OverlapResult.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(OverlapResult.GetActor()));
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	if(!IsValid(FirstActor) || !IsValid(SecondActor)) return false;
	
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName{"Player"}) && SecondActor->ActorHasTag(FName{"Player"});
	const bool bBothAreEnemy = FirstActor->ActorHasTag(FName{"Enemy"}) && SecondActor->ActorHasTag(FName{"Enemy"});

	const bool Friends = bBothAreEnemy || bBothArePlayers;

	return !Friends;
}

int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	if(!WorldContextObject) return 0;

	const UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	check(ClassInfo);

	const auto& DefaultClassInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
	return static_cast<int32>(DefaultClassInfo.XPReward.GetValueAtLevel(CharacterLevel));
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
	const auto& AuraTags = FAuraGameplayTags::Get();
	const AActor* SourceAvatarActor = Params.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = Params.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, Params.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, Params.KnockbackForce);

	SetIsRadialDamage(EffectContextHandle, Params.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, Params.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, Params.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, Params.RadialDamageOrigin);

	const FGameplayEffectSpecHandle EffectSpecHandle = Params.SourceAbilitySystemComponent->MakeOutgoingSpec(
		Params.DamageGameplayEffectClass, Params.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, Params.DamageType, Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraTags.Debuff_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraTags.Debuff_Damage, Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraTags.Debuff_Duration, Params.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraTags.Debuff_Frequency, Params.DebuffFrequency);
	
	Params.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	return EffectContextHandle;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if(NumRotators > 1)
	{
		Rotators.Reserve(NumRotators);
		
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}	
	}
	else
	{
		Rotators.Reserve(1);
		Rotators.Add(Forward.Rotation());
	}

	return Rotators;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlyRotatorVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if(NumVectors > 1)
	{
		Vectors.Reserve(NumVectors);
		
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}	
	}
	else
	{
		Vectors.Reserve(1);
		Vectors.Add(Forward);
	}

	return Vectors;
}

void UAuraAbilitySystemLibrary::GetClosestTargets(TArray<AActor*>& OutClosestActors, int32 MaxTargets,
	const TArray<AActor*>& Actors, const FVector& Origin)
{
	if(Actors.Num() <= MaxTargets)
	{
		OutClosestActors = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetToFound = 0;

	while (NumTargetToFound < MaxTargets)
	{
		if(ActorsToCheck.Num() == 0) break;
		
		double ClosesDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor = nullptr;
		
		for (const auto PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if(Distance < ClosesDistance)
			{
				ClosesDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}

		ActorsToCheck.Remove(ClosestActor);
		OutClosestActors.AddUnique(ClosestActor);
		
		NumTargetToFound++;
	}
}
