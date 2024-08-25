// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"

// Sets default values
AMagicCircle::AMagicCircle()
{
 	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicCircle::SetDecalMaterial(UMaterialInterface* DecalMaterial)
{
	MagicCircleDecal->SetMaterial(0, DecalMaterial);
}