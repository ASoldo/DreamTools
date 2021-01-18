#include "dtpch.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"
#include "../vendor/GLFW/include/GLFW/glfw3.h"
#include <chrono>

namespace DreamTools
{
	static uint32_t s_MapWidth = 6;

	static const char* s_MapTiles =
		"WWWWWW"
		"WWWWWW"
		"DDWDWD"
		"DDDDDD"
		"DDDDDD"
		"DDDDDD";

	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		DT_PROFILE_FUNCTION();

		m_CheckerBoardTexture = Texture2D::Create("assets/textures/DreamToolsCheckerboard.png");
		m_Logo = Texture2D::Create("assets/textures/DreamToolsLogo.png");
		m_SpriteSheet = Texture2D::Create("assets/game/textures/RPGpack.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = Framebuffer::Create(fbSpec);

		m_TextureStairs = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0,11 }, { 128,128 });

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

		s_TextureMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128,128 }, { 1,1 });
		s_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128,128 }, { 1,1 });


		m_CameraController.SetZoomLevel(5.0f);

		m_ActiveScene = CreateRef<Scene>();

		auto square = m_ActiveScene->CreateEntity("Square Soft Entity");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		auto square2 = m_ActiveScene->CreateEntity("Square Soft Infinity");
		square2.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_SquareEntity = square;
		m_CameraEntity = m_ActiveScene->CreateEntity("Main Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>().Primary = true;

		m_SecondCameraEntity = m_ActiveScene->CreateEntity("Second Camera Entity");
		auto& cc = m_SecondCameraEntity.AddComponent<CameraComponent>().Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				std::cout << "CameraController::OnCreate()" << std::endl;
			}

			void OnDestroy()
			{
				std::cout << "CameraController::OnDestroy()" << std::endl;
			}

			void OnUpdate(Timestep ts)
			{
				//std::cout << "Timestep " << ts << std::endl;
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;
				if (Input::IsKeyPressed(KeyCode::A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(KeyCode::D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(KeyCode::W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(KeyCode::S))
					translation.y -= speed * ts;

			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		//m_SecondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		//m_SceneHierarchyPanel.SetContext(m_ActiveScene);

	}
	void EditorLayer::OnDetach()
	{
		DT_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		DT_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (m_ViewPortFocused)
		{
			m_CameraController.OnUpdate(ts);
		}

		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();



		//static float rotation = 0.0f;
		//rotation += ts * 20.0f;

		//Renderer2D::BeginScene(m_CameraController.GetCamera());

		//Update scene
		m_ActiveScene->OnUpdate(ts);

		//for (uint32_t y = 0; y < m_MapHeight; y++)
		//{
		//	for (uint32_t x = 0; x < m_MapWidth; x++)
		//	{
		//		char tileType = s_MapTiles[x + y * m_MapWidth];
		//		Ref<SubTexture2D> texture;
		//		if (s_TextureMap.find(tileType) != s_TextureMap.end())
		//		{
		//			texture = s_TextureMap[tileType];
		//		}
		//		else
		//		{
		//			texture = m_TextureStairs;
		//		}
		//		Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight / 2.0f - y , 0.4f }, { 1.0f, 1.0f }, texture);
		//	}
		//}

		Renderer2D::EndScene();
		m_FrameBuffer->Unbind();

	}

	void EditorLayer::OnImGuiRender()
	{
		DT_PROFILE_FUNCTION();

		//-----------------------------------------------------------------------------
		// [SECTION] Example App: Docking, DockSpace / ShowExampleAppDockSpace()
		//-----------------------------------------------------------------------------

		// Demonstrate using DockSpace() to create an explicit docking node within an existing window.
		// Note that you already dock windows into each others _without_ a DockSpace() by just moving windows
		// from their title bar (or by holding SHIFT if io.ConfigDockingWithShift is set).
		// DockSpace() is only useful to construct to a central location for your application.

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("DreamTools"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				/*if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }*/
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::Separator();

				/*if (ImGui::MenuItem("Close", NULL, false))
					dockspaceOpen = false;*/
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		//CUSTOM UI START

		m_SceneHierarchyPanel.OnImGuiRender();


		ImGui::Begin("OpenGL Info:");
		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::Text("Version: %s", glGetString(GL_VERSION));
		ImGui::End();

		auto stats = Renderer2D::GetStats();
		ImGui::Begin("Renderer2D Stats:");
		ImGui::Text("DrawCalls: %d", stats.DrawCalls);
		ImGui::Text("QuadCount: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();
		//CUSTOM UI END

		ImGui::Begin("General Info:");
		//if (m_SquareEntity)
		//{
		//	auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
		//	ImGui::Text("%s", tag.c_str());
		//	auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
		//	ImGui::ColorEdit4("Square Color: ", glm::value_ptr(squareColor));
		//}
		//ImGui::DragFloat3("Camera Transform:",
		//	glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));

		if (ImGui::Checkbox("Toggle Camera", &m_PrimaryCamera))
		{
			m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		}

		//{
		//	auto& camera = m_SecondCameraEntity.GetComponent<CameraComponent>().Camera;
		//	float orthoSize = camera.GetOrthographicSize();
		//	if (ImGui::DragFloat3("2nd Camera Otrho Size: ", &orthoSize))
		//	{
		//		camera.SetOrthographicSize(orthoSize);
		//	}
		//}


		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("Viewport:");
		m_ViewPortFocused = ImGui::IsWindowFocused();
		m_ViewPortHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewPortFocused || !m_ViewPortHovered);
		//DT_CORE_WARN("Focused: {0}", ImGui::IsWindowFocused());
		//DT_CORE_WARN("Hovered: {0}", ImGui::IsWindowHovered());
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

		//-----------------------------------------------------------------------------
		// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()
		//-----------------------------------------------------------------------------
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}