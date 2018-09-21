// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRHGameMode.h"
#include "RHGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRH_API ARHGameMode : public AProjectRHGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

};
