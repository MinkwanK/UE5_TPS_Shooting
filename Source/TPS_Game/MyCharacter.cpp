// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MyCrosshair.h"
#include "MyProjectile.h"
#include "DrawDebugHelpers.h"

#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//게임 캐릭터가 컨트롤러의 회전 방향으로 같이 회전한다. 카메라의 방향에 캐릭터가 영향을 받음.
	//GetCharacterMovement()->bOrientRotationToMovement = true;


	TPS_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPS_Arm"));  //카메라 Arm
	TPS_Arm->SetupAttachment(RootComponent);
	TPS_Arm->TargetArmLength = 300.0f;
	TPS_Arm->SocketOffset.X = 60.0f;
	TPS_Arm->SocketOffset.Y = 60.0f;
	TPS_Arm->SocketOffset.Z = 100.0f;
	TPS_Arm->bUsePawnControlRotation = true; //상속받은 폰의 컨트롤 회전을 따를지를 말한다. 즉, 플레이어가 마우스를 움직여 회전하면 카메라도 같이 회전하다.


	TPS_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_Camera")); //카메라 
	TPS_Camera->SetupAttachment(TPS_Arm, USpringArmComponent::SocketName);


	
	

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//사격 타이머 설정. 자동사격. 1초단위.
	GetWorldTimerManager().SetTimer(FireRateHandle, this, &AMyCharacter::Fire, 0.1, true, 0);
	GetWorldTimerManager().PauseTimer(FireRateHandle); //처음에 중지한 상태로 시작하기 위해서이다.

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::Fire);
	PlayerInputComponent->BindAction("StopFire", IE_Released, this, &AMyCharacter::StopFire);
	//PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacter::DoCrouch);

	PlayerInputComponent->BindAxis("Move Forward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up", this, &APawn::AddControllerPitchInput);
}


void AMyCharacter::MoveForward(float Value)
{
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0); //카메라 축의 회전값 구하기
	const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation); //전방 벡터 구하기
	AddMovementInput(Direction, Value);
}

void AMyCharacter::MoveRight(float Value)
{
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0); //카메라 축의 회전값 구하기
	const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation); //전방 벡터 구하기
	AddMovementInput(Direction, Value);
}



// 총을 쏠때 타이머는 연사속도다. 한발쏘고 타이머가 끝나면 다시 한발쏘고의 예다.
//라인트레이스를 쏜다. 사격,상호작용에도 쓰인다.
void AMyCharacter::Fire()
{
	if (GetWorldTimerManager().IsTimerActive(FireRateHandle) == false)  //사격 명령이 중지됐다면
		GetWorldTimerManager().UnPauseTimer(FireRateHandle); //자동 사격 타이머 다시 시작

	FHitResult Hit;
	FQuat rotator = FQuat(Controller->GetControlRotation()); //FQuat는 Rotation 변수?
	FVector Start = this->GetMesh()->GetSocketLocation(FName("camera"));
	FVector End = Start + rotator.GetForwardVector() * 1000.0f; //스크린 중앙을 향하게 할 것이다.
	//End.Z += TPS_Camera->GetComponentLocation().Z - End.Z;

	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0, 0, 3.0);
	
	if (Rifle_FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Rifle_FireSound, GetActorLocation());
	}
	if (Rifle_FireParticles != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Rifle_FireParticles, this->GetMesh()->GetChildComponent(0)->GetSocketLocation(FName("MuzzleFlash")));
	}

}
void AMyCharacter::StopFire()
{
	GetWorldTimerManager().PauseTimer(FireRateHandle);
}

//void AMyCharacter::DoCrouch()
//{
//	UE_LOG(LogClass, Warning, TEXT("DoCrouch"));
//	CanCrouch() ? Crouch() : UnCrouch(); //웅크리기 가능하면 웅크리고 안되면 하지않기
//}



/*
void AMyCharacter::LineTraceFunc()
{
	FHitResult Hit;

	FQuat rotator = FQuat(Controller->GetControlRotation()); //FQuat는 Rotation 변수?
	FVector Start = this->GetMesh()->GetSocketLocation(FName("linetrace_socket"));
	FVector End = Start + rotator.GetForwardVector() * 1000.0f;

	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red,false,0,0,3.0);
}
*/



/*
void AMyCharacter::Fire()
{
	//발사시도
	if (ProjectileClass)
	{

		//FVector PlayerLocation;
		FVector PlayerLocation = this->GetMesh()->GetSocketLocation(FName("linetrace_socket")); //라인트레이스가 시작되는 위치, 즉 발사가 시작되는 위치
		FRotator PlayerRotation = Controller->GetControlRotation();


		GetActorEyesViewPoint(PlayerLocation, PlayerRotation);

		FVector MuzzleLocation = PlayerLocation + FTransform(PlayerRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = PlayerRotation;

		//MuzzleLocation.Z += 120.0;
		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this; //가해자가 누구인지?
			AMyProjectile* Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams); //투사체 스폰
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}
*/
