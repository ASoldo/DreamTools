#include <DreamTools.h>
#include "DreamTools/ImGui/ImGuiLayer.h"

#include "imgui/imgui.h"

#include "../vendor/GLFW/include/GLFW/glfw3.h"


//OpenGL Matehmatics Demo
//#include <glm/vec3.hpp> // glm::vec3
//#include <glm/vec4.hpp> // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
//{
//	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
//	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
//	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
//	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
//	return Projection * View * Model;
//}

class ExampleLayer : public DreamTools::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
		//OpenGL Matehmatics Demo
		//auto cam = camera(5.0f, { 0.5f, 0.5f });
	}
	void OnUpdate() override
	{
		//DT_CORE_INFO("ExampleLayer::OnUpdate");

		if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Tab))
		{
			DT_CORE_TRACE("Tab is pressed! (POLL)");
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Welcome, Aetrix!");
		ImGui::Text("Pick your favourite color");
		ImGui::ColorEdit4("", new float[4]);
		ImGui::End();

		ImGui::Begin("OpenGL Info:");
		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::Text("Version: %s", glGetString(GL_VERSION));
		ImGui::End();
	}

	void OnEvent(DreamTools::Event& event) override
	{
		if (event.GetEventType() == DreamTools::EventType::KeyPressed)
		{
			DreamTools::KeyPressedEvent& e = (DreamTools::KeyPressedEvent&)event;
			if (e.GetKeyCode() == DreamTools::Key::Tab)
			{
				DT_CORE_TRACE("{0} is pressed! (EVENT)", (char)e.GetKeyCode());
			}

			DT_CORE_TRACE("{0}", (char)e.GetKeyCode());
		}

		//DT_CORE_TRACE("{0}", event);
	}
};

class Sandbox : public DreamTools::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

DreamTools::Application* DreamTools::CreateApplication()
{
	return new Sandbox;
}