

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class AGOODSTART_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UProjectileMovementComponent* BulletProjectileMovement;
	
public:	

	
	
};
