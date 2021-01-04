#include <DreamTools.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "DreamTools/ImGui/ImGuiLayer.h"

#include "imgui/imgui.h"

#include "../vendor/GLFW/include/GLFW/glfw3.h"
#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"


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
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f) //, m_SquarePosition({0.0f, 0.0f, 0.0f})
	{
		//OpenGL Matehmatics Demo
		//auto cam = camera(5.0f, { 0.5f, 0.5f });

		m_VertexArray.reset(DreamTools::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		std::shared_ptr<DreamTools::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(DreamTools::VertexBuffer::Create(vertices, sizeof(vertices)));

		DreamTools::BufferLayout layout =
		{
			{ DreamTools::ShaderDataType::Float3, "a_Position" },
			{ DreamTools::ShaderDataType::Float4, "a_Color" },
		};


		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<DreamTools::IndexBuffer> indexBuffer;
		indexBuffer.reset(DreamTools::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(DreamTools::VertexArray::Create());


		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		std::shared_ptr<DreamTools::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(DreamTools::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		/*BufferLayout squareVertexBufferLayout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};*/

		squareVertexBuffer->SetLayout({ { DreamTools::ShaderDataType::Float3, "a_Position" } });
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<DreamTools::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(DreamTools::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		//Shader Source code (Vertex)
		std::string vertexSrc = R"(
			#version 330 core

			//vec3 is 12 bits
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
	
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				
			}
		)";

		//Shader Source code (Fragment)
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			
			void main()
			{
				//color = vec4(1.0, 0.5, 0.65, 1.0);
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";


		m_Shader.reset(DreamTools::Shader::Create(vertexSrc, fragmentSrc));

		//Shader2 start
		//Shader Source code (Vertex)
		std::string flatColorShaderVertexSrc = R"(
			#version 330 core

			//vec3 is 12 bits
			layout(location = 0) in vec3 a_Position;
	
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				
			}
		)";

		//Shader Source code (Fragment)
		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				//color = vec4(1.0, 0.5, 0.65, 1.0);
				//color = vec4(0.1, 0.2, 0.3, 1.0);
				color = vec4(u_Color, 1.0);
			}
		)";


		m_FlatColorShader.reset(DreamTools::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
		//Shader2 End

		//Index Buffer
		//Shader
	}
	void OnUpdate(DreamTools::Timestep ts) override
	{
		//DT_CORE_INFO("ExampleLayer::OnUpdate");

		/*if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Tab))
		{
			DT_CORE_TRACE("Tab is pressed! (POLL)");
		}*/

		//Print Delta Time
		//DT_CORE_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMillieconds());

		//Camera Movement Start
		if (DreamTools::Input::IsKeyPressed(DreamTools::Key::A))
		{
			m_CameraPosition.x -= m_CameraSpeed * ts;
		}
		else if (DreamTools::Input::IsKeyPressed(DreamTools::Key::D))
		{
			m_CameraPosition.x += m_CameraSpeed * ts;
		}
		if (DreamTools::Input::IsKeyPressed(DreamTools::Key::W))
		{
			m_CameraPosition.y += m_CameraSpeed * ts;
		}
		else if (DreamTools::Input::IsKeyPressed(DreamTools::Key::S))
		{
			m_CameraPosition.y -= m_CameraSpeed * ts;
		}
		//Camera Movement End

		//Move Square

		////Square Movement Start
		//if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Right))
		//{
		//	m_SquarePosition.x += m_SquareMoveSpeed * ts;
		//}
		//else if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Left))
		//{
		//	m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		//}
		//if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Down))
		//{
		//	m_SquarePosition.y -= m_SquareMoveSpeed * ts;
		//}
		//else if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Up))
		//{
		//	m_SquarePosition.y += m_SquareMoveSpeed * ts;
		//}
		////Square Movement End



		if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Q))
		{
			m_CameraRotation -= m_RotationSpeed * ts;
		}
		else if (DreamTools::Input::IsKeyPressed(DreamTools::Key::E))
		{
			m_CameraRotation += m_RotationSpeed * ts;
		}

		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		DreamTools::RenderCommand::Clear();


		//Set Camera Position
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		DreamTools::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<DreamTools::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<DreamTools::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				
				DreamTools::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
			}
		}
		/*glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition);
		DreamTools::Renderer::Submit(m_BlueShader, m_SquareVertexArray, transform);*/

		DreamTools::Renderer::Submit(m_Shader, m_VertexArray);

		DreamTools::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		

		ImGui::Begin("OpenGL Info:");
		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::Text("Version: %s", glGetString(GL_VERSION));
		ImGui::End();

		ImGui::Begin("Color Picker:");
		ImGui::ColorEdit3("Square Color:", glm::value_ptr(m_SquareColor));
		ImGui::End();

		/*ImGui::Begin("Timestep:");
		ImGui::Text("Seconds: %f", glGetFloatv();
		ImGui::Text("Milliseconds: %s", glGetString(GL_RENDERER));
		ImGui::End();*/
	}

	void OnEvent(DreamTools::Event& event) override
	{
		//OLD
		/*if (event.GetEventType() == DreamTools::EventType::KeyPressed)
		{
			DreamTools::KeyPressedEvent& e = (DreamTools::KeyPressedEvent&)event;
			if (e.GetKeyCode() == DreamTools::Key::Tab)
			{
				DT_CORE_TRACE("{0} is pressed! (EVENT)", (char)e.GetKeyCode());
			}

			DT_CORE_TRACE("{0}", (char)e.GetKeyCode());
		}*/
		//DT_CORE_TRACE("{0}", event);

		/*DreamTools::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<DreamTools::KeyPressedEvent>(DT_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));*/
	}
	//On Event Sample

	/*bool OnKeyPressedEvent(DreamTools::KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == DreamTools::Key::A)
		{
			m_CameraPosition.x += m_CameraSpeed;
		}
		else if (event.GetKeyCode() == DreamTools::Key::D)
		{
			m_CameraPosition.x -= m_CameraSpeed;
		}
		if (event.GetKeyCode() == DreamTools::Key::W)
		{
			m_CameraPosition.y -= m_CameraSpeed;
		}
		else if (event.GetKeyCode() == DreamTools::Key::S)
		{
			m_CameraPosition.y += m_CameraSpeed;
		}

		if (event.GetKeyCode() == DreamTools::Key::Q)
		{
			m_CameraRotation -= m_RotationSpeed;
		}
		else if (event.GetKeyCode() == DreamTools::Key::E)
		{
			m_CameraRotation += m_RotationSpeed;
		}
		return false;
	}*/

	private:
		std::shared_ptr<DreamTools::Shader> m_Shader;
		std::shared_ptr<DreamTools::VertexArray> m_VertexArray;

		std::shared_ptr<DreamTools::Shader> m_FlatColorShader;
		std::shared_ptr<DreamTools::VertexArray> m_SquareVertexArray;

		DreamTools::OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition;
		float m_CameraSpeed = 0.1f;
		float m_CameraRotation = 0.0f;
		float m_RotationSpeed = 10.0f;

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.4f };
		//Move Square
		//float m_SquareMoveSpeed = 0.1f;
		//glm::vec3 m_SquarePosition;
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