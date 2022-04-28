// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/Canvas.h"
#include "MyCrosshair.h"


void AMyCrosshair::DrawHUD()
{
	Super::DrawHUD();
	
	if (CrosshairTexture)
	{
		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		//�ؽ��� �߽��� ĵ���� �߽ɿ� �µ��� �ؽ����� ũ�� ���� ��ŭ �������� ��
		FVector2D CrosshairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth()*0.5), Center.Y - (CrosshairTexture->GetSurfaceHeight()*0.5));

		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);

	}
}