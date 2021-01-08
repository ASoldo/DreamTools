#include "dtpch.h"

#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"

#include "../vendor/GLFW/include/GLFW/glfw3.h"

#include <chrono>

template<typename Fn>

class Timer
{
public:
	Timer(const char* name, Fn&& func)
	: m_Name(name)
	, m_Func(func)
	, m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		if (!m_Stopped)
		{
			Stop();
		}
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}

private:
	Fn m_Func;
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	

};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);})

namespace DreamTools
{
	Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
	{
		m_CheckerBoardTexture = (DreamTools::Texture2D::Create("assets/textures/DreamToolsCheckerboard.png"));
	}

	void Sandbox2D::OnAttach()
	{
		
	}
	void Sandbox2D::OnDetach()
	{
	}
	void Sandbox2D::OnUpdate(DreamTools::Timestep ts)
	{
		PROFILE_SCOPE("Sandbox2D::OnUpdate");

		{
			PROFILE_SCOPE("m_CameraController.OnUpdate");
			m_CameraController.OnUpdate(ts);
		}
		

		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		DreamTools::RenderCommand::Clear();

		DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());
		DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 100.0f, 100.0f }, m_CheckerBoardTexture);
		DreamTools::Renderer2D::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.8f, 1.0f, 1.0f });
		DreamTools::Renderer2D::DrawQuad({ -0.5f, -1.5f, 0.0f }, { 1.0f, 1.0f }, { 0.4f, 0.8f, 1.0f, 1.0f });
		DreamTools::Renderer2D::DrawQuad({ -0.5f, -2.5f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.8f, 0.0f, 1.0f });
		
		DreamTools::Renderer2D::EndScene();
	}
	void Sandbox2D::OnImGuiRender()
	{
		ImGui::Begin("OpenGL Info:");
		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::Text("Version: %s", glGetString(GL_VERSION));
		ImGui::End();

		ImGui::Begin("Square Color:");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
		ImGui::End();

		ImGui::Begin("Profiling:");

		for (auto& result : m_ProfileResults)
		{
			char label[50];
			strcpy(label, "%.3fms ");
			strcat(label, result.Name);
			ImGui::Text(label, result.Time);
		}
		m_ProfileResults.clear();

		ImGui::End();
	}
	void Sandbox2D::OnEvent(DreamTools::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}