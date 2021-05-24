#include "hzpch.h"

#include "Hazel/Core/Window.h"
#include "Hazel/RHI/RHI.h"

#include "Platform/VulkanRHI/VulkanRHI.h"
// #include "Platform/D3D12RHI/D3D12RHI.h"

namespace Hazel {
	RHI* RHI::CreateRHI(Window* pWindow)
	{
		// At init, on windows

		HMODULE mod = GetModuleHandleA("renderdoc.dll");
		if (mod != NULL)
		{
			pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
			int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void**)&s_pRenderdoc_api);
			HZ_CORE_ASSERT(ret == 1, "Failed to fetch renderdoc.dll");
		}
		else
		{
			HZ_CORE_ERROR("Failed to initalize renderdoc error: {0}", GetLastError());
			// exit(0);
		}
		
		return new VulkanRHI(pWindow);
	}

}
