#include "SandboxRHI.h"


SandboxRHI::SandboxRHI()
{
	HZ_INFO("SandboxRHI Application layer is initalized");
}

void SandboxRHI::OnAttach()
{
	HZ_INFO("SandboxRHI Application layer is attached");
}

void SandboxRHI::OnDetach()
{
	HZ_INFO("SandboxRHI Application layer is Dettached, and destroyed");
}


void SandboxRHI::OnUpdate(Hazel::Timestep ts)
{

}

void SandboxRHI::OnImGuiRender()
{
	HZ_ERROR("ImGui is not yet implemented");
}

void SandboxRHI::OnEvent(Hazel::Event& e)
{
	HZ_CRITICAL("An Event is unhandled {0}", e.ToString());
}
