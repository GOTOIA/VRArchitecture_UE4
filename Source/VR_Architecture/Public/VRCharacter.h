// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"



class UCameraComponent;
class USceneComponent;
class UStaticMeshComponent;
class UPostProcessComponent;
class UMaterialInstanceDynamic;
class UMaterialInterface;
class UCurveFloat;
class UMotionControllerComponent;


UCLASS()
class VR_ARCHITECTURE_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	//Classe UEngine
	
	UPROPERTY()
	 UCameraComponent* Camera;

	UPROPERTY()
	 USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DestinationMarker;

	UPROPERTY(EditAnywhere)
	float TeleportProjectileRadius;

	UPROPERTY(EditAnywhere)
		float TeleportProjectileSpeed;

	UPROPERTY(EditAnywhere)
		float TeleportSimulationTime;

	UPROPERTY(EditAnywhere)
	float TeleportFadeTime;

	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtent;

	UPROPERTY()
		UPostProcessComponent *PostProcessComponent;

	UPROPERTY()
		UMaterialInstanceDynamic* BlinkerMaterialInstance;

	UPROPERTY(EditAnywhere)
		UMaterialInterface * BlinkerMaterialBase;


	UPROPERTY(EditAnywhere)
		UCurveFloat* RadiusVsVelocity;

	void UpdateDestinationMarker();

	bool FindTeleportDestination(FVector &OutLocation);

	void UpdateBlinkers();


	FVector2D GetBlinkerCentre();
	

	UPROPERTY()
		UMotionControllerComponent* LeftController;
	UPROPERTY()
		UMotionControllerComponent* RightController;


private :

	
	void MoveForward(float throttle);


	void MoveRight(float throttle);


	void BeginTeleport();

	void FinishTeleport();

	void StartFade(float FromAlpha, float ToAlpha);
	
};
