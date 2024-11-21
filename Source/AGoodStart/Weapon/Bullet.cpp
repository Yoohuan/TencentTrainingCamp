

#include "Bullet.h"


ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMesh;
	
	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletProjectileMovement"));

	BulletMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

