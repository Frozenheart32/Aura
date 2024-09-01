// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite, Category = "Aura FireBall")
	FDamageEffectParams ExplosionDamageEffectParams;

protected:

	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult) override;
	virtual void OnHit() override;

public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();
};
