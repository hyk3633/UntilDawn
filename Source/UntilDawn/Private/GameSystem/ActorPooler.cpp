

#include "GameSystem/ActorPooler.h"

UActorPooler::UActorPooler()
{
	PrimaryComponentTick.bCanEverTick = false;

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

