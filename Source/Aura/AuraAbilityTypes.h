#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{

	GENERATED_BODY()
	
public:

	bool IsBlockedHit() const { return  bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetBlockedHit(bool bInBlockedHit) { bIsBlockedHit = bInBlockedHit; }
	void SetIsCriticalHit(bool bInCriticalHit) { bIsCriticalHit = bInCriticalHit; } 

	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;
	UPROPERTY()
	bool bIsCriticalHit = false;
};
