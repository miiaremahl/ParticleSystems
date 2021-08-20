// 
//Miia Remahl . 18.08.2021 . Mushroom particle system.
//Uses Lance Putnam's particle system code as a code base and adds additional functionaly
//Also uses additional ez library from Lance Putnam
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ez/Random.h"
#include "MushroomParticles.generated.h"


// Particle that holds Unreal billboard
USTRUCT()
struct FABillboardParticle {
	GENERATED_BODY()

	FVector pos, vel, acc; //position, velocity, acceleration 
	float age = 0.; //age of particle
	float ageToDie = 0.; //When we want the particle to dissapear
	float scaleSize = 0.5; //size of particle

	// Update position of particle
	void update(float dt);
	void updateSecondary(float dt);

	UPROPERTY()
		class UBillboardComponent* billboard;
};


UCLASS()
class UABillboardParticles : public USceneComponent {
public:
	GENERATED_BODY()

	// Array of all the particles
	UPROPERTY()
		TArray<FABillboardParticle> particles;

	UPROPERTY()
		TArray<FABillboardParticle> secondaryParticles;

	float age = 0.; //Age of the entire system
	float ageToDie = 10.f; //When we stop updating entire particle system (every particle invisble at this point)


	// Update all particles
	void update(float dt);
};


UCLASS()
class MAGICFOREST_API AMushroomParticles : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMushroomParticles();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//For spawning particle system
	UFUNCTION(BlueprintCallable, Category = "Particle functions") //for testing purposes
		void SpawnParticles();

	//Initializing particles
	void InitializeParticles();

	//loading first particle system
	void LoadUpFirstSystem();

	//laoding second system
	void LoadUpSecondSystem();

	// The particle system
	UPROPERTY()
		UABillboardParticles* particleSystem;

	// Random number generator used for particle motion
	ez::Random<> rng;

	//is particle system triggered
	bool triggered = false;

	//helper for particle initialization
	bool particlesInitialized = false;

	//number of particles
	int numOfParticles = 30;
	int numOfSecondaryParticles = 40;

	//Textures
	TArray<UTexture2D*> uTextures;

	//Locations of the different textures
	TArray<wchar_t*> TextureLocations =
	{ TEXT("Texture'/Game/PollenBits1'"),
	  TEXT("Texture'/Game/PollenBits2'"),
	  TEXT("Texture'/Game/PollenBits3'"),
	  TEXT("Texture'/Game/PollenBits4'"),
	  TEXT("Texture'/Game/PollenBits5'"),
	  TEXT("Texture'/Game/PollenBits6'") };

};