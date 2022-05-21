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
	//���� ĳ���Ͱ� ��Ʈ�ѷ��� ȸ�� �������� ���� ȸ���Ѵ�. ī�޶��� ���⿡ ĳ���Ͱ� ������ ����.
	//GetCharacterMovement()->bOrientRotationToMovement = true;


	TPS_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPS_Arm"));  //ī�޶� Arm
	TPS_Arm->SetupAttachment(RootComponent);
	TPS_Arm->TargetArmLength = 300.0f;
	TPS_Arm->SocketOffset.X = 60.0f;
	TPS_Arm->SocketOffset.Y = 60.0f;
	TPS_Arm->SocketOffset.Z = 100.0f;
	TPS_Arm->bUsePawnControlRotation = true; //��ӹ��� ���� ��Ʈ�� ȸ���� �������� ���Ѵ�. ��, �÷��̾ ���콺�� ������ ȸ���ϸ� ī�޶� ���� ȸ���ϴ�.


	TPS_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_Camera")); //ī�޶� 
	TPS_Camera->SetupAttachment(TPS_Arm, USpringArmComponent::SocketName);


	
	

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//��� Ÿ�̸� ����. �ڵ����. 1�ʴ���.
	GetWorldTimerManager().SetTimer(FireRateHandle, this, &AMyCharacter::Fire, 0.1, true, 0);
	GetWorldTimerManager().PauseTimer(FireRateHandle); //ó���� ������ ���·� �����ϱ� ���ؼ��̴�.

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
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0); //ī�޶� ���� ȸ���� ���ϱ�
	const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation); //���� ���� ���ϱ�
	AddMovementInput(Direction, Value);
}

void AMyCharacter::MoveRight(float Value)
{
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0); //ī�޶� ���� ȸ���� ���ϱ�
	const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation); //���� ���� ���ϱ�
	AddMovementInput(Direction, Value);
}



// ���� �� Ÿ�̸Ӵ� ����ӵ���. �ѹ߽�� Ÿ�̸Ӱ� ������ �ٽ� �ѹ߽���� ����.
//����Ʈ���̽��� ���. ���,��ȣ�ۿ뿡�� ���δ�.
void AMyCharacter::Fire()
{
	if (GetWorldTimerManager().IsTimerActive(FireRateHandle) == false)  //��� ����� �����ƴٸ�
		GetWorldTimerManager().UnPauseTimer(FireRateHandle); //�ڵ� ��� Ÿ�̸� �ٽ� ����

	FHitResult Hit;
	FQuat rotator = FQuat(Controller->GetControlRotation()); //FQuat�� Rotation ����?
	FVector Start = this->GetMesh()->GetSocketLocation(FName("camera"));
	FVector End = Start + rotator.GetForwardVector() * 1000.0f; //��ũ�� �߾��� ���ϰ� �� ���̴�.
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
//	CanCrouch() ? Crouch() : UnCrouch(); //��ũ���� �����ϸ� ��ũ���� �ȵǸ� �����ʱ�
//}



/*
void AMyCharacter::LineTraceFunc()
{
	FHitResult Hit;

	FQuat rotator = FQuat(Controller->GetControlRotation()); //FQuat�� Rotation ����?
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
	//�߻�õ�
	if (ProjectileClass)
	{

		//FVector PlayerLocation;
		FVector PlayerLocation = this->GetMesh()->GetSocketLocation(FName("linetrace_socket")); //����Ʈ���̽��� ���۵Ǵ� ��ġ, �� �߻簡 ���۵Ǵ� ��ġ
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
			SpawnParams.Instigator = this; //�����ڰ� ��������?
			AMyProjectile* Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams); //����ü ����
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}
*/
