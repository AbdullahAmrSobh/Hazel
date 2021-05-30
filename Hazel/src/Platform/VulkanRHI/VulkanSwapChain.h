#pragma once
#include "Hazel/RHI/RHISwapChain.h"

#include <vulkan/vulkan.h>

namespace Hazel
{
	class VulkanDevice;
	class VulkanQueue;
	class VulkanRenderPass;
	class VulkanFrameBuffer;


	class VulkanSwapChain : public RHISwapChain
	{
	public:
		VulkanSwapChain(const VulkanDevice* pDevice, VkSurfaceKHR hSurfaces, const VulkanQueue* pQueue);
		~VulkanSwapChain();

		inline VkSwapchainKHR GetHandle() const { return m_hSwapchain; }

		inline VulkanRenderPass* GetRenderPass() const { return m_pRenderPass; }

		virtual void OnResize(uint32_t width, uint32_t height) final override;
		virtual void Present() final override;
		virtual RHIFrameBuffer* AcquireNextFrame() final override;

	private:

		void InitSwapchain(uint32_t width, uint32_t height);

		void InitRenderPass(VkFormat colorFormat);

		// void FreeFrameBuffers();
		void InitFrameBuffer(VkFormat format);

		VkExtent2D QueryCurrentExtent() const;
			
	private:
		const VulkanDevice*					m_pDevice;
		VkSwapchainKHR						m_hSwapchain;
		VkSurfaceKHR						m_hSurface;
		
		const VulkanQueue*					m_pPresentQueue;
		VulkanRenderPass*					m_pRenderPass;
		std::vector<VulkanFrameBuffer*>		m_pFrameBuffers;
		class VulkanSemaphore*				m_PresentCompleteSemaphore;

	};

}
