#include "dtpch.h"
#include "Sandbox2D.h"
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

	Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void Sandbox2D::OnAttach()
	{
		DT_PROFILE_FUNCTION();

		m_CheckerBoardTexture = DreamTools::Texture2D::Create("assets/textures/DreamToolsCheckerboard.png");
		m_Logo = DreamTools::Texture2D::Create("assets/textures/DreamToolsLogo.png");
		m_SpriteSheet = DreamTools::Texture2D::Create("assets/game/textures/RPGpack.png");

		DreamTools::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = DreamTools::Framebuffer::Create(fbSpec);

		m_TextureStairs = DreamTools::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0,11 }, { 128,128 });

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

		s_TextureMap['D'] = DreamTools::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128,128 }, { 1,1 });
		s_TextureMap['W'] = DreamTools::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128,128 }, { 1,1 });

		// Init here
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 3.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };

		m_CameraController.SetZoomLevel(5.0f);
	}
	void Sandbox2D::OnDetach()
	{
		DT_PROFILE_FUNCTION();

	}
	void Sandbox2D::OnUpdate(DreamTools::Timestep ts)
	{
		DT_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(ts);

		DreamTools::Renderer2D::ResetStats();

		m_FrameBuffer->Bind();

		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		DreamTools::RenderCommand::Clear();

		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		/*DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());		
			DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 20.0f, 20.0f }, m_CheckerBoardTexture, 1.0f);
			DreamTools::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.1f },{ 1.0f, 1.0f }, glm::radians(rotation), m_CheckerBoardTexture, 3.0f);
			DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			DreamTools::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.4f }, { 1.0f, 1.0f }, glm::radians(rotation) + 10.0f, m_Logo, 1.0f, {0.0f, 1.0f, 0.0f, 1.0f});
			DreamTools::Renderer2D::DrawRotatedQuad({ -0.5f, -2.5f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), { 0.2f, 0.8f, 0.6f, 1.0f });
		DreamTools::Renderer2D::EndScene();*/

		//DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//for (float y = -5.0f; y < 5.0f; y += 0.5f)
		//{
		//	for (float x = -5.0f; x < 5.0f; x += 0.5f)
		//	{
		//		glm::vec4 color = { (x + 5.0f) / 10, 0.4f, (y + 5.0f) / 10, 0.5f };
		//		DreamTools::Renderer2D::DrawQuad({ x, y, -0.1f }, { 0.45f, 0.45f }, color);
		//	}
		//}
		//DreamTools::Renderer2D::EndScene();
		
		if (DreamTools::Input::IsMouseButtonPressed(DreamTools::Mouse::ButtonLeft))
		{
			auto [x, y] = Input::GetMousePosition();
			auto width = DreamTools::Application::Get().GetWindow().GetWidth();
			auto height = DreamTools::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x , y + pos.y  };
			//m_Particle.Position = { 0, 0 };
			for (int i = 0; i < 5; i++)
				m_PartycleSystem.Emit(m_Particle);
		}

		DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth];
				DreamTools::Ref<DreamTools::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end())
				{
					texture = s_TextureMap[tileType];
				}
				else
				{
					texture = m_TextureStairs;
				}
				DreamTools::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight / 2.0f - y , 0.4f }, { 1.0f, 1.0f }, texture);
			}
		}
		DreamTools::Renderer2D::EndScene();
		m_FrameBuffer->Unbind();

		m_PartycleSystem.OnUpdate(ts);
		m_PartycleSystem.OnRender(m_CameraController.GetCamera());
	}
	void Sandbox2D::OnImGuiRender()
	{
		DT_PROFILE_FUNCTION();

		//-----------------------------------------------------------------------------
		// [SECTION] Example App: Docking, DockSpace / ShowExampleAppDockSpace()
		//-----------------------------------------------------------------------------

		// Demonstrate using DockSpace() to create an explicit docking node within an existing window.
		// Note that you already dock windows into each others _without_ a DockSpace() by just moving windows
		// from their title bar (or by holding SHIFT if io.ConfigDockingWithShift is set).
		// DockSpace() is only useful to construct to a central location for your application.
		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
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
				if (ImGui::BeginMenu("Options"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
					ImGui::MenuItem("Padding", NULL, &opt_padding);
					ImGui::Separator();

					if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
					if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
					if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
					if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
					if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
					if (ImGui::MenuItem("Exit")) DreamTools::Application::Get().Close();
					ImGui::Separator();

					/*if (ImGui::MenuItem("Close", NULL, false))
						dockspaceOpen = false;*/
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			//CUSTOM UI START

			ImGui::Begin("OpenGL Info:");
			ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
			ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
			ImGui::Text("Version: %s", glGetString(GL_VERSION));
			ImGui::End();

			auto stats = DreamTools::Renderer2D::GetStats();
			ImGui::Begin("Renderer2D Stats:");
			ImGui::Text("DrawCalls: %d", stats.DrawCalls);
			ImGui::Text("QuadCount: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::Begin("Scene:");
				uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
				ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });
			ImGui::End();

			ImGui::End();
			//CUSTOM UI END

			ImGui::End();

			//-----------------------------------------------------------------------------
			// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()
			//-----------------------------------------------------------------------------

		}
	}

	void Sandbox2D::OnEvent(DreamTools::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}