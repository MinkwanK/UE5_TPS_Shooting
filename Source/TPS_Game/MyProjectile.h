// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "MyProjectile.generated.h"

UCLASS()
class TPS_GAME_API AMyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile) 
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = Movement) //blueprintreadwrite로 해야 비주얼에서 편집시 블루프린트에더 적용이된다.
	UProjectileMovementComponent* ProjectileMovementComponent;

	// 발사체의 속도를 발사 방향으로 초기화시킵니다.
	void FireInDirection(const FVector& ShootDirection);
	void ProjectileDestroy();

};
