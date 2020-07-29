#include "hzpch.h"
#include "Scene.h"
#include "Hazel/Scene/Components.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Scene/Entity.h"

#include <glm/glm.hpp>

namespace Hazel {
	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


		if (m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);


		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* mainCameraTransform = nullptr;
		{
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					mainCameraTransform = &m_Registry.get<TransformComponent>(entity).Transform;
					break;
				}
			}

		}
		
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *mainCameraTransform);
			auto view = m_Registry.view<TransformComponent>();
			for (auto entity : view)
			{
				if (m_Registry.has<SpriteRendererComponent>(entity))
				{
					TransformComponent& transform = view.get<TransformComponent>(entity);
					SpriteRendererComponent sprite = m_Registry.get<SpriteRendererComponent>(entity);
					Renderer2D::DrawQuad(transform, sprite.Color);
				}
			}
			Renderer2D::EndScene();
		}
	}
}