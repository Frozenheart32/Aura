// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations() const
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for(int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		                                     Location, Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Green, 4.f);
		                                     
		const FVector ChosenSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 5.f, 12, FColor::Cyan, false,3.f);
		  
		
		DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false,3.f);
		DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false,3.f);

		SpawnLocations.Add(ChosenSpawnLocation);
	}
	
	return SpawnLocations;
}
