#pragma once
#include "Hazel/RHI/RHI.h"


#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanCommandBuffer.h"


namespace Hazel {
	
	class VulkanSwapChain;

	class VulkanRHI : public RHI
	{
	public:
		VulkanRHI(Window* pWindow);
		~VulkanRHI();

		virtual Ref<RHIShader> CreateShader(const RHIPrecompiledShader& shader) const final override;

		virtual Scope<RHIPipelineLayout> CreatePipelineLayout(const RHIPipelineLayoutDesc& layoutDesc) final override;
		virtual Scope<RHIGraphicsPipelineState> CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& psoDesc) final override;

		virtual std::vector<Scope<RHICommandBuffer>> AllocateCommandBuffer(uint32_t count) final override;
		virtual void SubmitCommandBuffer(RHICommandBuffer* pCommandBuffer) final override;

		virtual RHISwapChain* GetSwapChain();
	private:
		inline std::vector<const char*> EnabledLayers()
		{
			std::vector<const char*> layers = {
				"VK_LAYER_KHRONOS_validation",
				"VK_LAYER_LUNARG_api_dump",
			};

			return layers;
		}

		inline std::vector<const char*> EnabledExtensions()
		{
			std::vector<const char*> extensions;

			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			uint32_t count;
			const char** pData = glfwGetRequiredInstanceExtensions(&count);
			for (uint32_t index = 0; index < count; index++) extensions.push_back(pData[index]);

			return extensions;
		}

	private:
		VkInstance					m_InstanceHandle;
		VkDebugUtilsMessengerEXT	m_DebugMessengerHandle;
		VkSurfaceKHR				m_SurfaceHandle;
		
		VulkanSwapChain*			m_pSwapChain;

		VulkanDevice				m_Device;
		
		VulkanCommandsAllocator*	m_pCommandsAllocator;
	};
}
