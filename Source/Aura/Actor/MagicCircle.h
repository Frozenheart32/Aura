// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class UDecalComponent;

UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:
	
	AMagicCircle();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;
	
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;
};
