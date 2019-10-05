// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"


class UCameraComponent;
class USceneComponent;
class UStaticMeshComponent;

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
	float MaxTeleportDistance = 1000.f;

	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 1;

	void UpdateDestinationMarker();


public:
	UFUNCTION(BlueprintCallable, Category = "Move")
	void MoveForward(float throttle);

	UFUNCTION(BlueprintCallable, Category = "Move")
	void MoveRight(float throttle);


	void BeginTeleport();

	void FinishTeleport();
	
};
