#include <DreamTools.h>

class Sandbox : public DreamTools::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

DreamTools::Application* DreamTools::CreateApplication()
{
	return new Sandbox;
}