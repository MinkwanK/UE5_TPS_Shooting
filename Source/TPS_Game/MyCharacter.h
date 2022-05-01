#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyCharacter.generated.h"

UCLASS()
class TPS_GAME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TPS_Camera) //blueprintreadonly�� blueprint���� �������� ������ ������ �ȵȴ�.
		class UCameraComponent* TPS_Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TPS_Arm)
		class USpringArmComponent* TPS_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	//UPROPERTY(EditDefaultsOnly, Category = Projectile)
	//TSubclassOf<class AMyProjectile>ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AMyCrosshair>CrosshairClass;

	//EditAnywhere �����ڸ� ���� �ѱ� ������ ���� �������Ʈ �������� ����Ʈ ��峪 ĳ������ �ƹ� �ν��Ͻ��� ���� ������ �ǿ��� ������ �� �ֽ��ϴ�. 
	//BlueprintReadWrite �����ڸ� ���ؼ��� �������Ʈ �ȿ��� �ѱ� �������� ���� ���ϰ� ������ �� �ֽ��ϴ�.
	//EditDefaultsOnly �����ڴ� ������Ÿ�� Ŭ������ �������Ʈ�� �� �ν��Ͻ� �󿡼��� �ƴ϶� �������Ʈ�� ����Ʈ�θ� ������ �� �ִٴ� ���Դϴ�.	
	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	//UFUNCTION()
	//void LineTraceFunc();

	UFUNCTION()
		void Fire();
	UFUNCTION()
		void StopFire();
	//UFUNCTION()
	//void DoCrouch(); //��ũ����

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	FTimerHandle FireRateHandle;


};