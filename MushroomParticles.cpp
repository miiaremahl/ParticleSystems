// 
//Miia Remahl . 18.08.2021 . Mushroom particle system.
//Uses Lance Putnam's particle system code as a code base and adds additional functionaly
//Also uses additional ez library from Lance Putnam
#include "MushroomParticles.h"
#include "Components/BillboardComponent.h"
#include <string>
#include <algorithm> 
#include "ez/Util.h"
#include "ez/Texture.h"
#include "ez/Random.h"

//Updating one particle
void FABillboardParticle::update(float dt) {
	//Updating velocity,position, acceleration
	vel -= FVector(0.f, 0.f, dt * 5.f); //slowing down during time
	pos += vel * dt;
	age += dt;

	//scale particle over time
	//making sure the scale wont to under 0
	if (scaleSize > 0.01f && (scaleSize - 0.01f) > 0.01f) {
		scaleSize -= 0.05f * dt;
	}

	billboard->SetEditorScale(scaleSize);

	//set new location
	if (billboard) billboard->SetRelativeLocation(pos);

	//check if particle should die
	if (age >= ageToDie) {
		billboard->SetHiddenInGame(true);
	}
}


//Updating secondary system particle
void FABillboardParticle::updateSecondary(float dt) {
	//Updating velocity,position,acceleration
	vel += FVector(0.f, 0.f, dt);
	pos += vel * dt;
	age += dt;

	//scale particle over time
	//making sure the scale wont to under 0
	if (scaleSize > 0.01f && (scaleSize - 0.01f) > 0.01f) {
		scaleSize -= 0.02f * dt;
	}

	billboard->SetEditorScale(scaleSize);

	//set new location
	if (billboard) billboard->SetRelativeLocation(pos);

	//check if particle should die
	if (age >= ageToDie) {
		billboard->SetHiddenInGame(true);
	}
}

//Updating particle system
void UABillboardParticles::update(float dt) {

	age += dt;

	//calling every particles update
	for (auto& p : particles) p.update(dt);

	for (auto& p : secondaryParticles) p.updateSecondary(dt);

}

// Sets default values
AMushroomParticles::AMushroomParticles()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creating both systems
	EZ_CREATE_DEFAULT_SUBOBJECT(particleSystem);
	RootComponent = particleSystem;

	//load particle systems
	LoadUpFirstSystem();
	LoadUpSecondSystem();
}


// Load particle system 1
void AMushroomParticles::LoadUpFirstSystem()
{
	//Loading textures
	for (int i = 0; i < TextureLocations.Num(); i++)
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> asset1(TextureLocations[i]);
		if (asset1.Succeeded()) {
			uTextures.Add(asset1.Object);
		}
	}

	//reference to particle system
	auto& particles = particleSystem->particles;

	// Set the number of particles in our system
	particles.SetNum(numOfParticles);

	// Initialize particles
	for (int i = 0; i < particles.Num(); ++i) {
		auto& p = particles[i];

		// Generate a name for the particle
		auto name = FName(("p" + std::to_string(i)).c_str());

		// Create the billboard
		p.billboard = CreateDefaultSubobject<std::decay<decltype(*p.billboard)>::type>(name);

		//Making sure the textures are loaded (caused crashing earlier)
		if (uTextures.Num() > 0) {
			//Pick random texture index (6 textures)
			int textureIndex = FMath::RandRange(0, uTextures.Num() - 1);
			p.billboard->Sprite = uTextures[textureIndex];;

		}

		// Attach particle to particle system component
		p.billboard->SetupAttachment(particleSystem);

		//needed for billboards
		p.billboard->bHiddenInGame = false;

		//However hide them w this for now
		p.billboard->SetHiddenInGame(true);

		p.billboard->SetEditorScale(0.5);

	}
}



// Load particle system 2 (similar to the first one, but keeping code separate in case of changes wanted to  only one)
void AMushroomParticles::LoadUpSecondSystem() {

	//loading the textures
	ConstructorHelpers::FObjectFinder<UTexture2D> asset(TEXT("Texture'/Game/PollenBall6'"));
	if (asset.Succeeded()) {
		UTexture2D* texture = asset.Object;

		//reference to particle system
		auto& particles2 = particleSystem->secondaryParticles;

		// Set the number of particles in our system
		particles2.SetNum(numOfSecondaryParticles);

		// Initialize particles
		for (int i = 0; i < particles2.Num(); ++i) {
			auto& p = particles2[i];

			// Generate a name for the particle (sp=secondary particle)
			auto name = FName(("sp" + std::to_string(i)).c_str());

			// Create the billboard
			p.billboard = CreateDefaultSubobject<std::decay<decltype(*p.billboard)>::type>(name);

			//sprite setting -> setting by one texture
			p.billboard->Sprite = texture;

			// Attach particle to particle system component
			p.billboard->SetupAttachment(particleSystem);

			//needed for billboards
			p.billboard->bHiddenInGame = false;

			//However hide them w this for now
			p.billboard->SetHiddenInGame(true);

		}
	}
}


// Called when the game starts or when spawned
void AMushroomParticles::BeginPlay()
{
	Super::BeginPlay();
}


void AMushroomParticles::SpawnParticles()
{
	//the particle system triggers once -> lets set it triggered 
	triggered = true;

}

void AMushroomParticles::InitializeParticles()
{
	//first particle system
	for (auto& p : particleSystem->particles) {
		p.billboard->SetHiddenInGame(false); //make visible
		p.age = 0.; //setting age to 0 at start 
		p.pos = FVector(0.0f, 0.0f, 0.0f); // All the particles leave from same spot

		float vUP = FMath::RandRange(1.0f, 5.f); //velocity up -> to move particles up
		float vSide = FMath::RandRange(-1.1f, 1.1f); //random movement to the sides
		float xSide = FMath::RandRange(-1.1f, 1.1f); //random movement to the sides

		p.vel = FVector(xSide, vSide, vUP) * 6;  //going up -> add velocity up
		p.acc = FVector(0.0f, 0.0f, 0.0f); //Acceleration  -> we dont want it to go down so no acceleration
		p.ageToDie = FMath::RandRange(7.0f, 9.0f); //choosing randomly how long the particle is going to live
		p.scaleSize = FMath::RandRange(0.1f, 0.2f); //random size for the particle
	}

	//secondary particles
	for (auto& p2 : particleSystem->secondaryParticles) {
		p2.billboard->SetHiddenInGame(false); //make visible
		p2.age = 0.; //setting age to 0 at start 
		p2.pos = FVector(0.0f, 0.0f, 0.0f); // All the particles leave from same spot

		float vUP = FMath::RandRange(1.0f, 5.f); //velocity up -> to move particles up
		float vSide = FMath::RandRange(-2.0f, 2.0f); //random movement to the sides
		float xSide = FMath::RandRange(-2.0f, 2.0f); //random movement to the sides

		p2.vel = FVector(xSide, vSide, vUP) * 5;  //going up -> add velocity up
		p2.acc = FVector(0.0f, 0.0f, 0.0f); //Acceleration  -> we dont want it to go down so no acceleration
		p2.ageToDie = FMath::RandRange(8.0f, 10.0f); //choosing randomly how long the particle is going to live
		p2.scaleSize = FMath::RandRange(0.1f, 0.2f); //random size for the particle

	}
}


// Called every frame
void AMushroomParticles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Only update if the particle system is triggered
	if (triggered) {

		//I made the Inizialization here because when I did it in the constructor, the tick didnt have the values setup like it should
		//If you have an idea why this was let me know
		if (!particlesInitialized) {
			InitializeParticles();
			particlesInitialized = true;
		}

		UE_LOG(LogTemp, Warning, TEXT("numbser of particles: %i"), particleSystem->particles.Num());

		// Add some jitter to each particle
		for (auto& p : particleSystem->particles) {
			float vUP = FMath::RandRange(-0.3f, 0.3f);
			float vSide = FMath::RandRange(-0.3f, 0.3f); //random movement to the sides
			float xSide = FMath::RandRange(-0.3f, 0.3f); //random movement to the sides
			p.vel += FVector(xSide, vSide, vUP);
		}

		//check if the system is alive
		if (particleSystem->age >= particleSystem->ageToDie) {
			//stop updating the system if every particle is gone
			triggered = false;
		}

		// Update particle positions
		particleSystem->update(DeltaTime);
	}
}
