// Fill out your copyright notice in the Description page of Project Settings.

#include "VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"





// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Root Component
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestinationMarker"));
	DestinationMarker->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	DestinationMarker->SetVisibility(false);
	
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0.f;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);
	UpdateDestinationMarker();
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AVRCharacter::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Released, this, &AVRCharacter::BeginTeleport);

}

bool AVRCharacter::FindTeleportDestination(FVector &OutLocation)
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * MaxTeleportDistance;


	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

	if (!bHit) return false;

	FNavLocation NavLocation;
	//TODO Fix
	//bool bOnNavMesh = GetWorld()->GetNavigationSystem()->ProjectPointToNavigation(HitResult.Location, NavLocation, TeleportProjectionExtent);

	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	bool bHitNav = false;

	if (NavSystem != nullptr) {
		bHitNav = NavSystem->ProjectPointToNavigation
		(
			HitResult.Location,
			NavLocation,
			TeleportProjectionExtent
		);
	}
	

	if (!bHitNav) return false;

	OutLocation = NavLocation.Location;
	return true;
}

void AVRCharacter::UpdateDestinationMarker()
{
	FVector Location;
	bool bHasDestnation = FindTeleportDestination(Location);

	if (bHasDestnation)
	{
		DestinationMarker->SetVisibility(true);

		DestinationMarker->SetWorldLocation(Location);
	}
	else
	{
		DestinationMarker->SetVisibility(false);
	}
}

void AVRCharacter::MoveForward(float throttle)
{
	AddMovementInput(throttle * Camera->GetForwardVector());
}

void AVRCharacter::MoveRight(float throttle)
{
	AddMovementInput(throttle * Camera->GetRightVector());
}

void AVRCharacter::BeginTeleport()
{

	StartFade(0, 1);
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AVRCharacter::FinishTeleport, TeleportFadeTime);
}

void AVRCharacter::FinishTeleport()
{
	SetActorLocation(DestinationMarker->GetComponentLocation() + GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	StartFade(1, 0);

}


void AVRCharacter::StartFade(float FromAlpha, float ToAlpha)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC != nullptr)
	{
		PC->PlayerCameraManager->StartCameraFade(FromAlpha, ToAlpha, TeleportFadeTime, FLinearColor::Black);
	}
}