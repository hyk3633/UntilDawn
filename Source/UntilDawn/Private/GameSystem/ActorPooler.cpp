

#include "GameSystem/ActorPooler.h"

UActorPooler::UActorPooler()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UActorPooler::SetActorClass(UClass* newClass)
{
	actorClass = newClass;
}

void UActorPooler::SpawnPoolableActor(const int spawnCount)
{
	check(actorClass);

	AActor* actor = nullptr;
	for (int i = 0; i < spawnCount; i++)
	{
		actor = GetWorld()->SpawnActor<AActor>(actorClass, FVector(0, 0, -3500), FRotator::ZeroRotator);
		actorPool.Add(actor);
	}
}

TWeakObjectPtr<AActor> UActorPooler::GetPooledActor()
{
	IPoolableActor* poolableActor = nullptr;
	for (auto actor : actorPool)
	{
		poolableActor = Cast<IPoolableActor>(actor);
		if (poolableActor->IsActorActivated() == false)
		{
			poolableActor->ActivateActor();
			return MakeWeakObjectPtr<AActor>(actor);
		}
	}
	SpawnPoolableActor(3);
	
	return nullptr;
}

