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
		VulkanSwapChain(const VulkanDevice* pDevice, const VulkanQueue* pQueue, const VkSwapchainCreateInfoKHR& createInfo);
		~VulkanSwapChain();

		inline VkSwapchainKHR GetHandle() const { return m_Handle; }

		inline VulkanRenderPass* GetRenderPass() const { return m_pRenderPass; }

		virtual void OnResize(uint32_t width, uint32_t height) final override;
		virtual void Present() final override;
		virtual void SwapBuffers() final override;

		virtual RHIFrameBuffer** GetFrameBuffer() final override;

	private:

		void RetriveTheSwapChainImages(VkFormat imageFormat);
		void InitRenderPass(VkFormat colorFormat);
		void InitFrameBuffer(VkFormat format);
			
	private:
		const VulkanDevice*				m_pDevice;
		const VulkanQueue*				m_pPresentQueue;
		VkSwapchainKHR					m_Handle;
		VulkanRenderPass*				m_pRenderPass;
		std::vector<VulkanFrameBuffer*> m_pFrameBuffers;
	};
}
