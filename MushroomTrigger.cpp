//
//Miia Remahl . 19.08.2021 . Mushroom trigger box.
//Followed Harrison McGuire's tutorial on triggerboxes to implemented trigger funtionality: https://www.youtube.com/watch?v=Ck3OE_olUr0
////For playing animation sequences, KeySmash Studious: https://www.youtube.com/watch?v=9w12_RSwr3o
//
#include "MushroomTrigger.h"
#include "MushroomParticles.h"

AMushroomTrigger::AMushroomTrigger() {

	//Making sure our code is calling overlap functions when they happen
	OnActorBeginOverlap.AddDynamic(this, &AMushroomTrigger::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AMushroomTrigger::OnOverlapEnd);
}

void AMushroomTrigger::BeginPlay() {

	Super::BeginPlay();
}

//When overlapping begins
void AMushroomTrigger::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor){

	if (OtherActor) {
		if (TargetStaticMushroom->SequencePlayer) {
			TargetStaticMushroom->SequencePlayer->Play(); //play the animation
			particlesystem->SpawnParticles();
		}
	}

}

//When overlapping ends
void AMushroomTrigger::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor){
	UE_LOG(LogTemp, Warning, TEXT("Overlap ended"));
}