#pragma once
#include <vulkan/vulkan.h>
#include "Hazel/RHI/RHISwapChain.h"

#include "Platform/VulkanRHI/VulkanRenderPass.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"

namespace Hazel {

	class VulkanDevice;

	struct VulkanSwapChainDesc
	{
		VkSurfaceKHR					surface;
		VkSurfaceFormatKHR				surfaceFormat;
		VkPresentModeKHR				presentMode;
		VkExtent2D						swapcainExtent;
		VkSurfaceTransformFlagBitsKHR	transform;
		uint32_t						minImageCount;
	};

	class VulkanSwapChain : public RHISwapChain
	{
	public:
		VulkanSwapChain(const VulkanDevice* pDevice, const VulkanSwapChainDesc& desc);
		~VulkanSwapChain();

		virtual void SwapFrameBuffers() final override;
		virtual void Present() final override;

		virtual uint32_t GetCurrentFrameBufferIndex() final override;
		virtual uint32_t GetBackBuffersCount() final override;
		virtual RHIFrameBuffer** GetFrameBuffers() final override;

		inline const VulkanRenderPass* GetRenderPass() const { return &m_RenderPass; }

	private:
		const VulkanDevice*				m_pDevice;
		VkSwapchainKHR					m_SwapChainHandle;
		VulkanRenderPass				m_RenderPass;
		uint32_t						m_CurrentFrameBufferIndex;
		std::vector<VulkanFrameBuffer>  m_FrameBuffers;
		std::vector<VkSemaphore>		m_WaitSemaphoresHandles;

		VkSemaphore						m_ImageAvailableSemaphore;
	};
}
