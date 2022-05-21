// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnBox.generated.h"

UCLASS()
class TPS_GAME_API ASpawnBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnBox, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnBox, meta = (AllowPrivateAccess = "true"))
		//class UBoxComponent* CollisionBox;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
