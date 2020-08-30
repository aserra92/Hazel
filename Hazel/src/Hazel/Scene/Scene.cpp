#include "hzpch.h"
#include "Scene.h"
#include "Hazel/Scene/Components.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Scene/Entity.h"

#include <glm/glm.hpp>

namespace Hazel {
	Scene::Scene()
	{
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
		//Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}
					nsc.Instance->OnUpdate(ts);
				});
		}

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

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		//Resize our non-FixedAspectRation cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}
}
