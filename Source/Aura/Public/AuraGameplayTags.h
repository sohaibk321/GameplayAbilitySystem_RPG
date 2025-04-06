// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
 * Aura Gameplay Tags
 *
 * Singleton containing native gameplay tags * 
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();
protected:
	
private:
	static FAuraGameplayTags GameplayTags;
};
