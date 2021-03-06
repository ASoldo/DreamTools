#include <DreamTools.h>
#include "DreamTools/Core/EntryPoint.h"

//#include "Platform/OpenGL/OpenGLShader.h"
//#include "DreamTools/ImGui/ImGuiLayer.h"
//
//#include "imgui/imgui.h"
//
//#include "../vendor/GLFW/include/GLFW/glfw3.h"
//#include "../glm/glm/gtc/matrix_transform.hpp"
//#include "../glm/glm/gtc/type_ptr.hpp"

#include "EditorLayer.h"


//class ExampleLayer : public DreamTools::Layer
//{
//public:
//	ExampleLayer() : Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
//	{
//		//OpenGL Matehmatics Demo
//		//auto cam = camera(5.0f, { 0.5f, 0.5f });
//
//		m_VertexArray = DreamTools::VertexArray::Create();
//
//		float vertices[4 * 7] = {
//			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
//			0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//			-0.5f, 0.5, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f
//		};
//		DreamTools::Ref<DreamTools::VertexBuffer> vertexBuffer = DreamTools::VertexBuffer::Create(vertices, sizeof(vertices));
//
//		DreamTools::BufferLayout layout =
//		{
//			{ DreamTools::ShaderDataType::Float3, "a_Position" },
//			{ DreamTools::ShaderDataType::Float4, "a_Color" },
//		};
//
//
//		vertexBuffer->SetLayout(layout);
//		m_VertexArray->AddVertexBuffer(vertexBuffer);
//
//		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
//		DreamTools::Ref<DreamTools::IndexBuffer> indexBuffer = DreamTools::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
//		m_VertexArray->SetIndexBuffer(indexBuffer);
//
//		m_SquareVertexArray = DreamTools::VertexArray::Create();
//
//
//		float squareVertices[5 * 4] = {
//			-0.5f, -0.5f, 0.0f, 0.0F, 0.0f,
//			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
//			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
//		};
//
//		std::shared_ptr<DreamTools::VertexBuffer> squareVertexBuffer = DreamTools::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
//
//		/*BufferLayout squareVertexBufferLayout =
//		{
//			{ ShaderDataType::Float3, "a_Position" },
//			{ ShaderDataType::Float4, "a_Color" },
//		};*/
//
//		squareVertexBuffer->SetLayout({ 
//			{ DreamTools::ShaderDataType::Float3, "a_Position" },
//			{ DreamTools::ShaderDataType::Float2, "a_TexCoord" },
//		});
//		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);
//
//		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
//		DreamTools::Ref<DreamTools::IndexBuffer> squareIndexBuffer = DreamTools::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
//		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);
//
//		//Shader Source code (Vertex)
//		std::string vertexSrc = R"(
//			#version 330 core
//
//			//vec3 is 12 bits
//			layout(location = 0) in vec3 a_Position;
//			layout(location = 1) in vec4 a_Color;
//
//			uniform mat4 u_ViewProjection;
//			uniform mat4 u_Transform;
//	
//			out vec3 v_Position;
//			out vec4 v_Color;
//
//			void main()
//			{
//				v_Position = a_Position;
//				v_Color = a_Color;
//				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
//				
//			}
//		)";
//
//		//Shader Source code (Fragment)
//		std::string fragmentSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) out vec4 color;
//
//			in vec3 v_Position;
//			in vec4 v_Color;
//
//			
//			void main()
//			{
//				//color = vec4(1.0, 0.5, 0.65, 1.0);
//				color = vec4(v_Position * 0.5 + 0.5, 1.0);
//				color = v_Color;
//			}
//		)";
//
//
//		m_Shader = DreamTools::Shader::Create("VertexColoring", vertexSrc, fragmentSrc);
//
//		//Shader2 start
//		//Shader Source code (Vertex)
//		std::string flatColorShaderVertexSrc = R"(
//			#version 330 core
//
//			//vec3 is 12 bits
//			layout(location = 0) in vec3 a_Position;
//	
//			uniform mat4 u_ViewProjection;
//			uniform mat4 u_Transform;
//
//			out vec3 v_Position;
//
//			void main()
//			{
//				v_Position = a_Position;
//				
//				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
//				
//			}
//		)";
//
//		//Shader Source code (Fragment)
//		std::string flatColorShaderFragmentSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) out vec4 color;
//
//			in vec3 v_Position;
//
//			uniform vec3 u_Color;
//
//			void main()
//			{
//				//color = vec4(1.0, 0.5, 0.65, 1.0);
//				//color = vec4(0.1, 0.2, 0.3, 1.0);
//				color = vec4(u_Color, 1.0);
//			}
//		)";
//
//
//		m_FlatColorShader = DreamTools::Shader::Create("FlatColorShader", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);
//		//Shader2 End
//
//		//TextureShader start
//		
//		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
//		//TextureShader End
//		//Assign to m_Texture
//		m_Texture = (DreamTools::Texture2D::Create("assets/textures/DreamToolsCheckerboard.png"));
//		m_LogoTexture = (DreamTools::Texture2D::Create("assets/textures/DreamToolsLogo.png"));
//
//		std::dynamic_pointer_cast<DreamTools::OpenGLShader>(textureShader)->Bind();
//		std::dynamic_pointer_cast<DreamTools::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
//	}
//
//
//	void OnUpdate(DreamTools::Timestep ts) override
//	{
//		//Print Delta Time
//		//DT_CORE_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMillieconds());
//
//		////Square Movement Start
//		//if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Right))
//		//{
//		//	m_SquarePosition.x += m_SquareMoveSpeed * ts;
//		//}
//		//else if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Left))
//		//{
//		//	m_SquarePosition.x -= m_SquareMoveSpeed * ts;
//		//}
//		//if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Down))
//		//{
//		//	m_SquarePosition.y -= m_SquareMoveSpeed * ts;
//		//}
//		//else if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Up))
//		//{
//		//	m_SquarePosition.y += m_SquareMoveSpeed * ts;
//		//}
//		////Square Movement End
//
//		//Update
//		m_CameraController.OnUpdate(ts);
//		
//		//Renderer
//		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
//		DreamTools::RenderCommand::Clear();
//
//		DreamTools::Renderer::BeginScene(m_CameraController.GetCamera());
//
//		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
//
//		std::dynamic_pointer_cast<DreamTools::OpenGLShader>(m_FlatColorShader)->Bind();
//		std::dynamic_pointer_cast<DreamTools::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
//
//		m_FlatColorShader->Bind();
//		DreamTools::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
//
//		DreamTools::Renderer::EndScene();
//	}
//
//	virtual void OnImGuiRender() override
//	{
//		ImGui::Begin("OpenGL Info:");
//		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
//		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
//		ImGui::Text("Version: %s", glGetString(GL_VERSION));
//		ImGui::End();
//
//		ImGui::Begin("Grid Color:");
//		ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));
//		ImGui::End();
//
//		/*ImGui::Begin("Timestep:");
//		ImGui::Text("Seconds: %f", glGetFloatv();
//		ImGui::Text("Milliseconds: %s", glGetString(GL_RENDERER));
//		ImGui::End();*/
//	}
//
//	void OnEvent(DreamTools::Event& e) override
//	{
//		m_CameraController.OnEvent(e);
//	}
//	
//	private:
//
//		DreamTools::ShaderLibrary m_ShaderLibrary;
//		DreamTools::Ref<DreamTools::Shader> m_Shader;
//		DreamTools::Ref<DreamTools::VertexArray> m_VertexArray;
//
//		DreamTools::Ref<DreamTools::Shader> m_FlatColorShader;
//		DreamTools::Ref<DreamTools::VertexArray> m_SquareVertexArray;
//
//		DreamTools::Ref<DreamTools::Texture2D> m_Texture;
//		DreamTools::Ref<DreamTools::Texture2D> m_LogoTexture;
//
//		DreamTools::OrthographicCameraController m_CameraController;
//
//		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.4f };
//
//};
namespace DreamTools
{
	class Dreamer : public Application
	{
	public:
		Dreamer() : Application("Dreamer")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}
		~Dreamer()
		{

		}
	};

	Application* CreateApplication()
	{
		return new Dreamer;
	}
}