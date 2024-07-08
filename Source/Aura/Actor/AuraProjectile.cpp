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

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuraProjectile::Destroyed()
{
	if(!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect,GetActorLocation());

		if(LoopingSoundComponent)
			LoopingSoundComponent->Stop();
		
		bHit = true;
	}

	
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if(!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
		return;

	if(!UAuraAbilitySystemLibrary::IsNotFriend(OtherActor, DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser()))
		return;
	

	if(!bHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect,GetActorLocation());
		if(LoopingSoundComponent)
			LoopingSoundComponent->Stop();
	}
	
	if(HasAuthority())
	{
		if(DamageEffectSpecHandle.IsValid())
		{
			if(const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
			}
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

