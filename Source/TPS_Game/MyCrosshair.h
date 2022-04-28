// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyCrosshair.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAME_API AMyCrosshair : public AHUD
{
	GENERATED_BODY()

public:
	// HUD �� ���� �ֿ� ��� ���Դϴ�.
	virtual void DrawHUD() override;

protected:
	// ȭ�� �߾ӿ� �׷��� ���Դϴ�.
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;
	
	
};