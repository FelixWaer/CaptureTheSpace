// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"

#include "PlayerSpaceShip.h"
#include "Kismet/GameplayStatics.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerSpaceShip = Cast<APlayerSpaceShip>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
}
