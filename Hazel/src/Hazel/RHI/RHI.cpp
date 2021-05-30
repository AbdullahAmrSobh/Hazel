#include "hzpch.h"

#include "Hazel/Core/Window.h"
#include "Hazel/RHI/RHI.h"

#include "Platform/VulkanRHI/VulkanRHI.h"

namespace Hazel {
	Scope<RHI> RHI::CreateRHI(Window* pWindow)
	{
		return CreateScope<VulkanRHI>(pWindow);
	}

}
