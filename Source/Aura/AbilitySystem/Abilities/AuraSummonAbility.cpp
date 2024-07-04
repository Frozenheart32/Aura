// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations(bool bIsDebug) const
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
		                                     
		FVector ChosenSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector{0.f, 0.f, 400}, ChosenSpawnLocation + FVector{0.f, 0.f, -400.f}, ECC_Visibility);


		if(Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		
		if(bIsDebug)
		{
			DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 5.f, 12, FColor::Cyan, false,3.f);
			DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false,3.f);
			DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false,3.f);
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
	}
	
	return SpawnLocations;
}
