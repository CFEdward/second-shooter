// Fill out your copyright notice in the Description page of Project Settings.

#include "Explosive.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AExplosive::AExplosive() :
	Damage(50.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosiveMesh"));
	SetRootComponent(ExplosiveMesh);

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosive::BulletHit_Implementation(FHitResult HitResult, AController* ShooterController, AActor* Shooter)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
	if (ExplodeParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplodeParticles,
			HitResult.Location,
			FRotator(0.f),
			true);
	}

	// Apply explosive damage
	TArray<AActor*> OverlappingCharacters;
	// Get all actors that overlap with the explosive sphere
	// also overlaps with the enemy AggroSphere, which is not good
	GetOverlappingActors(OverlappingCharacters, ACharacter::StaticClass());

	for (auto Character : OverlappingCharacters)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor damaged by explosive: %s"), *Character->GetName());
		
		// So this time we check which components of the overlapped Actor/Character
		// are also overlapping with the explosive sphere
		TArray<UPrimitiveComponent*> OverlappingComponents;
		GetOverlappingComponents(OverlappingComponents);
		for (auto Component : OverlappingComponents)
		{
			// And we only apply damage to the character if the explosive sphere is overlapping with the Capsule
			// of said Actor/Character
			auto IsCapsule = Cast<UCapsuleComponent>(Component);
			if (IsCapsule)
			{
				UGameplayStatics::ApplyDamage(
					Character,
					Damage,
					ShooterController,
					Shooter,
					UDamageType::StaticClass());
			}
		}
	}

	Destroy();
}
