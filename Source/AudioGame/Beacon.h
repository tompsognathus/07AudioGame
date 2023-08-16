// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Beacon.generated.h"

UCLASS()
class AUDIOGAME_API ABeacon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeacon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beacon")
	bool AutoPinging = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beacon")
	float PingInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beacon")
	bool IsCollectable = false;

	UFUNCTION(BlueprintCallable, Category = "Beacon")
	bool GetIsCollectable() { return IsCollectable; }


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
