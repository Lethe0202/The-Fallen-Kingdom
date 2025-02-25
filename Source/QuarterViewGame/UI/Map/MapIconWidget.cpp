// Fill out your copyright notice in the Description page of Project Settings.


#include "MapIconWidget.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Image.h"

void UMapIconWidget::SetMapIcon(const TSoftObjectPtr<UTexture2D>& MapIconTexture)
{
	UTexture2D* MapIconTexture2D = MapIconTexture.LoadSynchronous();
	
	if (MapIconTexture.IsValid())
	{
		IconImage->SetVisibility(ESlateVisibility::Visible);
		UMaterialInstanceDynamic* ImageMaterialInstanceDynamic = IconImage->GetDynamicMaterial();
		ImageMaterialInstanceDynamic->SetTextureParameterValue("Icon", Cast<UTexture>(MapIconTexture2D));
	}
	else
	{
		IconImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMapIconWidget::SetMapColorAndOpacity(const FColor& InColorAndOpacity)
{
	IconImage->SetColorAndOpacity(InColorAndOpacity);
}
