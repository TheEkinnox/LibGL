#pragma once
#include "Entity.h"
#include "DataStructure/Graph.h"

namespace LibGL::Resources
{
	class Scene : public DataStructure::Graph<Entity>
	{
	public:
		virtual void update();
	};
}
