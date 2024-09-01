// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AAuraProjectile::AAuraProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);

    Sphere->SetCollisionObjectType(ECC_Projectile);
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
    Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 550.f;
	MovementComponent->MaxSpeed = 550.f;
	MovementComponent->ProjectileGravityScale = 0.f;
}


void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuraProjectile::Destroyed()
{
	if(!bHit && !HasAuthority())
	{
		OnHit();
	}
	
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if(!IsValidOverlap(OtherActor)) return;

	if(!bHit)
	{
		OnHit();
	}
	
	if(HasAuthority())
	{
		if(const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const bool bIsKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if(bIsKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				
				const FVector KnockbackDirection = Rotation.Vector();
				DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
			}
			
			DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect,GetActorLocation());

	if(LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
		
	bHit = true;
}

bool AAuraProjectile::IsValidOverlap(AActor* OtherActor) const
{
	if(DamageEffectParams.SourceAbilitySystemComponent == nullptr)
		return false;

	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	if(!IsValid(SourceAvatarActor) || SourceAvatarActor == OtherActor)
		return false;

	if(!UAuraAbilitySystemLibrary::IsNotFriend(OtherActor, SourceAvatarActor))
		return false;


	return true;
}

