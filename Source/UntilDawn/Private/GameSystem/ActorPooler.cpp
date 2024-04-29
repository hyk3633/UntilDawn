

#include "GameSystem/ActorPooler.h"

UActorPooler::UActorPooler()
{
	PrimaryComponentTick.bCanEverTick = false;

}

bool UActorPooler::IsActorTypeExist(const int itemType)
{
	return actorPoolMap.Find(itemType) != nullptr;
}

void UActorPooler::SpawnPoolableActor(const int itemType, UClass* actorClass, const int spawnCount)
{
	if (actorPoolMap.Find(itemType) == nullptr)
	{
		actorPoolMap.Emplace(itemType);
	}

	AActor* actor = nullptr;
	for (int i = 0; i < spawnCount; i++)
	{
		actor = GetWorld()->SpawnActor<AActor>(actorClass, FVector(0, 0, -3500), FRotator::ZeroRotator);
		actorPoolMap[itemType].actorPool.Add(actor);
	}
}

TWeakObjectPtr<AActor> UActorPooler::GetPooledActor(const int itemType)
{
	IPoolableActor* poolableActor = nullptr;
	for (auto actor : actorPoolMap[itemType].actorPool)
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

