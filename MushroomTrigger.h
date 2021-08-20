//
//Miia Remahl . 19.08.2021 . Mushroom trigger box.
//Followed Harrison McGuire's tutorial on triggerboxes to implemented trigger funtionality: https://www.youtube.com/watch?v=Ck3OE_olUr0
//For playing animation sequences, KeySmash Studious: https://www.youtube.com/watch?v=9w12_RSwr3o
//

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "MushroomTrigger.generated.h"


UCLASS()
class MAGICFOREST_API AMushroomTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override; //Overriding beginplay

public:
	AMushroomTrigger();

	// overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// overlap end function
	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	//Mushroom actor
	UPROPERTY(EditAnywhere) 
		ALevelSequenceActor* TargetStaticMushroom;

};
