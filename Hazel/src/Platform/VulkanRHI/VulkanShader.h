#pragma once
#include "Hazel/RHI/RHIShader.h"
#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanDevice.h"

namespace Hazel {

	class VulkanShader : public RHIShader
	{
	public:
		VulkanShader(const VulkanDevice* pDevice, VkShaderModule moduleHandle)
			: m_pDevice(pDevice), m_ModuleHandle(moduleHandle) 
		{ 
		}

		VulkanShader(const VulkanShader& other) = delete;
		
		VulkanShader(VulkanShader&& other) noexcept
			: m_pDevice(other.m_pDevice), m_ModuleHandle(other.m_ModuleHandle)
		{
			other.m_pDevice = VK_NULL_HANDLE;
		}

		~VulkanShader()
		{
			if(m_pDevice != nullptr) 
				vkDestroyShaderModule(m_pDevice->GetHandle(), m_ModuleHandle, nullptr);
		}

		inline VkShaderModule GetHandle() const { return m_ModuleHandle; }

	private:
		const VulkanDevice* m_pDevice;
		VkShaderModule m_ModuleHandle;
	};
}
