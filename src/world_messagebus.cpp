#include "world_messagebus.h"

namespace Retro3D
{
	void WorldMessageBus::ActorAdded(Actor* arg_actor)
	{
		mActorsAdded.push_back(arg_actor);
	}

	void WorldMessageBus::ComponentAdded(Component* arg_comp)
	{
		mComponentsAdded.push_back(arg_comp);
	}
}
