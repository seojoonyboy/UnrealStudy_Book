// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	boxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	boxComp->SetCollisionProfileName(TEXT("Bullet"));

	//박스 컴포넌트 충돌 오버랩 이벤트에 BulletOverlap 함수를 연결한다.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::ABullet::OnBulletOverlap);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation() + GetActorForwardVector() * moveSpeed * DeltaTime;
	SetActorLocation(newLocation);
}

void ABullet::OnBulletOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AEnemyActor* enemy = Cast<AEnemyActor>(OtherActor);

	if(enemy != nullptr)
	{
		//충돌한 액터 제거
		OtherActor->Destroy();

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			explosionFx,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	//자기자신 제거
	Destroy();
}

