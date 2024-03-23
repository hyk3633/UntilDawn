

#include "GameSystem/ActorPooler.h"

UActorPooler::UActorPooler()
{
	PrimaryComponentTick.bCanEverTick = false;

}

AActor* UActorPooler::GetPooledActor()
{
	IPoolableActor* poolableActor = nullptr;
	for (AActor* actor : actorPool)
	{
		poolableActor = Cast<IPoolableActor>(actor);
		if (poolableActor->IsActorActivated() == false)
		{
			poolableActor->ActivateActor();
			return actor;
		}
	}
	return nullptr;
}

