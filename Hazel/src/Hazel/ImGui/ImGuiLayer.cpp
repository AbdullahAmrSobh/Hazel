#include "hzpch.h"
#include "Hazel/ImGui/ImGuiLayer.h"

// #include <imgui.h>

#include "Hazel/Core/Application.h"
#include "Hazel/RHI/RendererHardwareInterface.h"


namespace Hazel {

	ImGuiLayer::ImGuiLayer(RHI* pRHI)
		: Layer("ImGuiLayer")
		, m_pRHI(pRHI)
	{
	}

	void ImGuiLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

	}

	void ImGuiLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();

	}

	void ImGuiLayer::OnEvent(Event& e)
	{

	}
	
	void ImGuiLayer::Begin()
	{
		HZ_PROFILE_FUNCTION();
		return;
	}

	void ImGuiLayer::End()
	{
		HZ_PROFILE_FUNCTION();

	}

}
