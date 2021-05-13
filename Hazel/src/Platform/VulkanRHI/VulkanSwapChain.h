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

		inline VkSwapchainKHR GetHandle() const { return m_SwapChainHandle; }

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
		const VulkanQueue*				m_pQueue;
		VkSwapchainKHR					m_SwapChainHandle;
		uint32_t						m_Width;
		uint32_t						m_Height;
		VulkanRenderPass*				m_pRenderPass;
		std::vector<VkImage>			m_SwapChainImages;
		std::vector<VulkanFrameBuffer*> m_pFrameBuffers;
	};
}
