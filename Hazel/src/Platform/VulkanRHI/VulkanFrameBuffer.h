#pragma once

#include "Hazel/RHI/RHIFrameBuffer.h"

#include <vulkan/vulkan.h>
#include "Platform/VulkanRHI/VulkanResources.h"

namespace Hazel {

	class VulkanDevice;

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
		
		inline const VulkanSemaphore& GetImageAvailableSemaphore() const { return m_FrameAvailableSemaphore; }


	private:
		const VulkanDevice*			m_pDevice;
		VkFramebuffer				m_FrameBufferHandle;
		const VulkanRenderPass*		m_pRenderPass;
		VkImage						m_ImageHandle;
		std::vector<VkImageView>	m_Views;

		VulkanSemaphore				m_FrameAvailableSemaphore;
	};
}
