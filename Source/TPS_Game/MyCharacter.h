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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TPS_Camera) //blueprintreadonly는 blueprint에서 읽을수는 있지만 변경은 안된다.
		class UCameraComponent* TPS_Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TPS_Arm)
		class USpringArmComponent* TPS_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	//UPROPERTY(EditDefaultsOnly, Category = Projectile)
	//TSubclassOf<class AMyProjectile>ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AMyCrosshair>CrosshairClass;

	//EditAnywhere 지정자를 통해 총구 오프셋 값을 블루프린트 에디터의 디폴트 모드나 캐릭터의 아무 인스턴스에 대한 디테일 탭에서 변경할 수 있습니다. 
	//BlueprintReadWrite 지정자를 통해서는 블루프린트 안에서 총구 오프셋의 값을 구하고 설정할 수 있습니다.
	//EditDefaultsOnly 지정자는 프로젝타일 클래스를 블루프린트의 각 인스턴스 상에서가 아니라 블루프린트의 디폴트로만 설정할 수 있다는 뜻입니다.	
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
	//void DoCrouch(); //웅크리기

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	FTimerHandle FireRateHandle;


};