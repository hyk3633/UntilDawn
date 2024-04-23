

#include "GameSystem/ActorPooler.h"

UActorPooler::UActorPooler()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UActorPooler::SpawnPoolableActor(UClass* actorClass, const int spawnCount)
{
	AActor* actor = nullptr;
	for (int i = 0; i < spawnCount; i++)
	{
		actor = GetWorld()->SpawnActor<AActor>(actorClass, FVector(0, 0, -3500), FRotator::ZeroRotator);
		actorPool.Add(actor);
	}
}

TArray<AActor*>& UActorPooler::GetActorPool()
{
	return actorPool;
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
	return nullptr;
}

