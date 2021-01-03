#include "dtpch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
//#include "../vendor/GLFW/include/GLFW/glfw3.h"
#include <glad/glad.h>
#include "DreamTools/Renderer/Renderer.h"
#include "DreamTools/Renderer/RenderCommand.h"

#include "Input.h"

namespace DreamTools
{
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	
	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		DT_CORE_ASSERT(!s_Instance, "Application is already running!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};


		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(VertexArray::Create());
		

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f, 
			 0.75f,  0.75f, 0.0f, 
			-0.75f,  0.75f, 0.0f, 
		};

		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		
		/*BufferLayout squareVertexBufferLayout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};*/

		squareVertexBuffer->SetLayout({ { ShaderDataType::Float3, "a_Position" } });
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		//Shader Source code (Vertex)
		std::string vertexSrc = R"(
			#version 330 core

			//vec3 is 12 bits
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
	
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				
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


		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		//Shader2 start
		//Shader Source code (Vertex)
		std::string blueShaderVertexSrc = R"(
			#version 330 core

			//vec3 is 12 bits
			layout(location = 0) in vec3 a_Position;
	
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				
			}
		)";

		//Shader Source code (Fragment)
		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				//color = vec4(1.0, 0.5, 0.65, 1.0);
				color = vec4(0.1, 0.2, 0.3, 1.0);
			}
		)";


		m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
		//Shader2 End

		//Index Buffer
		//Shader
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		DT_CORE_TRACE("{0}",e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		/*if (e.IsInCategory(EventCategoryApplication))
		{
			DT_CLIENT_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			DT_CLIENT_TRACE(e);
		}*/
		

		while (m_Running)
		{

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();


			//Set Camera Position
			//m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			m_Camera.SetRotation(45.0f);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_BlueShader, m_SquareVertexArray);

			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			//Debug Draw Mouse Position 
			//auto [x, y] = Input::GetMousePosition();
			//DT_CORE_TRACE("{0} {1}", x, y);

			m_Window->OnUpdate();
		}
	}
}