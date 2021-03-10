#include "hzpch.h"
#include "Hazel/Core/Window.h"
#include "Hazel/RHI/RHI.h"

#include "Platform/VulkanRHI/VulkanRHI.h"

namespace Hazel {
	RHI* RHI::CreateRHI(Window* pWindow)
	{
		return new VulkanRHI(pWindow);
	}
}
