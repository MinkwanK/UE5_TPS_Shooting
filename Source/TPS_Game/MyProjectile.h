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
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = Movement) //blueprintreadwrite�� �ؾ� ���־󿡼� ������ �������Ʈ���� �����̵ȴ�.
	UProjectileMovementComponent* ProjectileMovementComponent;

	// �߻�ü�� �ӵ��� �߻� �������� �ʱ�ȭ��ŵ�ϴ�.
	void FireInDirection(const FVector& ShootDirection);
	void ProjectileDestroy();

};
