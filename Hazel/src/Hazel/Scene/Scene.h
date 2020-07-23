#pragma once

#include "entt.hpp"


#include "Hazel/Core/TimeStep.h"

namespace Hazel {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(TimeStep ts);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}







