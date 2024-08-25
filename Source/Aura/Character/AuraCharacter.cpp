// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator{0.f, 400.f, 0.f};
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init ability actor info for the client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	LevelUpParticles_Multicast();
}

void AAuraCharacter::LevelUpParticles_Multicast_Implementation() const
{
	if(IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();

		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AAuraCharacter::GetXP_Implementation()
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddLevel(InPlayerLevel);

	const auto AuraASC = CastChecked<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
	AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePointsCount)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToAttributePoints(InAttributePointsCount);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPointsCount)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToSpellPoints(InSpellPointsCount);
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if(const auto AuraPC = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPC->ShowMagicCircle(DecalMaterial);
		AuraPC->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if(const auto AuraPC = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPC->HideMagicCircle();
		AuraPC->bShowMouseCursor = true;
	}
}

void AAuraCharacter::InitAbilityActorInfo()
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	const auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent.Get());
	
	check(AuraASC);
	
	AuraASC->AbilityActorInfoSet();
	AuraASC->InitAbilityActorInfo(AuraPlayerState, this);

	OnASCRegistered.Broadcast(AbilitySystemComponent);

	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AAuraCharacter::StunTagChanged);

	if(const auto AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(const auto AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent.Get(), AttributeSet.Get());
		}
	}

	InitializeDefaultAttributes();
}

void AAuraCharacter::OnRep_Stunned()
{
	if(const auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		const auto& AuraTags = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(AuraTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(AuraTags.Player_Block_InputHeld);
		BlockedTags.AddTag(AuraTags.Player_Block_InputPressed);
		BlockedTags.AddTag(AuraTags.Player_Block_InputReleased);

		if(bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if(bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}
