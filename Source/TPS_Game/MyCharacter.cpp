// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
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
	TPS_Arm->bUsePawnControlRotation = true; //상속받은 폰의 컨트롤 회전을 따를지를 말한다. 즉, 플레이어가 마우스를 움직여 회전하면 카메라도 같이 회전하다.


	TPS_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_Camera")); //카메라 
	TPS_Camera->SetupAttachment(TPS_Arm, USpringArmComponent::SocketName);



}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LineTraceFunc();


}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::Fire);
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

/*
void AMyCharacter::LineTraceFunc()
{
	FHitResult Hit;

	FQuat rotator = FQuat(Controller->GetControlRotation()); //FQuat는 Rotation 변수?
	FVector Start = this->GetMesh()->GetSocketLocation(FName("linetrace_socket"));
	FVector End = Start + rotator.GetForwardVector() * 100.0f;

	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
}
*/

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
