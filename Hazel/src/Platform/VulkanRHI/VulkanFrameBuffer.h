#pragma once

#include "Hazel/RHI/RHIFrameBuffer.h"

#include <vulkan/vulkan.h>

namespace Hazel {

	class VulkanDevice;
	class VulkanSemaphore;

	class VulkanRenderPass : public RHIRenderPass
	{
	public:
		VulkanRenderPass(const VulkanDevice* pDevice, const std::vector<VkAttachmentDescription>& attachmentsDesc, const std::vector<VkSubpassDescription>& subpassesDesc, const std::vector<VkSubpassDependency>& dependenciesDesc);
		~VulkanRenderPass();

		inline VkRenderPass GetHandle() const { return m_RenderPass; }

	private:
		const VulkanDevice* m_pDevice;
		VkRenderPass m_RenderPass;
	};

	class VulkanFrameBuffer : public RHIFrameBuffer
	{
	public:
		VulkanFrameBuffer(
			const VulkanDevice* pDevice,
			uint32_t width,
			uint32_t height,
			VkImage imageHandle,
			const VulkanRenderPass* pRenderPass,
			const std::vector<VkImageView>& attachments
		);

		~VulkanFrameBuffer();

		inline VkFramebuffer GetHandle() const { return m_FrameBufferHandle; }
		inline VkImage GetImage() const { return m_ImageHandle; }
		inline const std::vector<VkImageView>&	GetViews() const { return m_Views; }
		inline const VulkanRenderPass* GetRenderPass() const { return m_pRenderPass; }
		inline void AddRenderingSemaphore(VkSemaphore semaphore) { m_RenderingSemaphores.push_back(semaphore); }
		
		inline VkSemaphore GetImageAvailableSemaphore() const { return m_ImageAvailableSemaphore; }


	private:
		const VulkanDevice*			m_pDevice;
		VkFramebuffer				m_FrameBufferHandle;
		const VulkanRenderPass*		m_pRenderPass;
		VkImage						m_ImageHandle;
		std::vector<VkImageView>	m_Views;

		std::vector<VulkanSemaphore*> m_Semaphores;

		std::vector<VkSemaphore>	m_RenderingSemaphores;
		VkSemaphore					m_ImageAvailableSemaphore;
	};
}
