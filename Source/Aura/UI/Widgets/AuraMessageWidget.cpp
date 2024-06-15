// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/AuraMessageWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


void UAuraMessageWidget::SetMessage(UTexture2D* NewIcon, const FText& NewText)
{
	if(Image_Icon)
	{
		Image_Icon->SetBrushFromTexture(NewIcon);
	}

	if(TextBlock_Message)
	{
		TextBlock_Message->SetText(NewText);
	}
}
