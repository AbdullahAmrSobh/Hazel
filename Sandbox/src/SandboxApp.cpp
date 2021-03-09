#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "SandboxRHI.h"

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new SandboxRHI());
	}

	~Sandbox()
	{
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
