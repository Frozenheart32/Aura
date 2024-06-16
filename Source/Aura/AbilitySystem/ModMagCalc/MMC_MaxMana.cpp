// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntellingenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntellingenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntellingenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntellingenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float IntelligenceMagnitude = 0.f;
	GetCapturedAttributeMagnitude(IntellingenceDef, Spec, EvaluationParameters, IntelligenceMagnitude);

	IntelligenceMagnitude = FMath::Max(IntelligenceMagnitude, 0.f);

	const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.5f * IntelligenceMagnitude + 15.f * PlayerLevel;
}
