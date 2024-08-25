// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#define CREATE_POINT(Point)										\
	Point = CreateDefaultSubobject<USceneComponent>(#Point);	\
	ImmutablePts.Add(Point);									\

// Sets default values
APointCollection::APointCollection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CREATE_POINT(Pt_0);
	SetRootComponent(Pt_0);

	CREATE_POINT(Pt_1);
	Pt_1->SetupAttachment(GetRootComponent());
	
	CREATE_POINT(Pt_2);
	Pt_2->SetupAttachment(GetRootComponent());
	
	CREATE_POINT(Pt_3);
	Pt_3->SetupAttachment(GetRootComponent());
	
	CREATE_POINT(Pt_4);
	Pt_4->SetupAttachment(GetRootComponent());
	
	CREATE_POINT(Pt_5);
	Pt_5->SetupAttachment(GetRootComponent());
	
	CREATE_POINT(Pt_6);
	Pt_6->SetupAttachment(GetRootComponent());
	
	CREATE_POINT(Pt_7);
	Pt_7->SetupAttachment(GetRootComponent());
	
	CREATE_POINT(Pt_8);
	Pt_8->SetupAttachment(GetRootComponent());
	
	CREATE_POINT(Pt_9);
	Pt_9->SetupAttachment(GetRootComponent());
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,
	float YawOverride) const
{
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("Attempted to access ImmutablePoints out of bounds"));
	
	TArray<USceneComponent*> Result;
	for (USceneComponent* Pt : ImmutablePts)
	{
		if(Result.Num() >= NumPoints) return Result;

		if(Pt != Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);

			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector{Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f};
		const FVector LoweredLocation = FVector{Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f};


		FHitResult HitResult;
		TArray<AActor*> Players;
		
		UAuraAbilitySystemLibrary::GetLivePlayersWithRadius(this, Players, {}, 1500.f,  GetActorLocation());

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(Players);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName{"BlockAll"}, QueryParams);

		const FVector AdjustedLocation = FVector{Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z};
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		Result.Add(Pt);
	}

	return Result;
}

// Called when the game starts or when spawned
void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

