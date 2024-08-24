// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:

	UPassiveNiagaraComponent();

protected:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;


	virtual void BeginPlay() override;

private:

	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
};
