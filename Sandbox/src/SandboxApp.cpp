#include <DreamTools.h>

class ExampleLayer : public DreamTools::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}
	void OnUpdate() override
	{
		DT_CORE_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(DreamTools::Event& event) override
	{
		DT_CORE_TRACE("{0}", event);
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