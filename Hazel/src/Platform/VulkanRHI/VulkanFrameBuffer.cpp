#include "hzpch.h"

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanResources.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"



namespace Hazel {

	VulkanRenderPass::VulkanRenderPass(
		const VulkanDevice* pDevice,
		const std::vector<VkAttachmentDescription>& attachmentsDesc, 
		const std::vector<VkSubpassDescription>& subpassesDesc, 
		const std::vector<VkSubpassDependency>& dependenciesDesc
	)
		: m_pDevice(pDevice)
		, m_RenderPass(VK_NULL_HANDLE)
	{
		VkRenderPassCreateInfo createInfo	= {};
		createInfo.sType					= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.pNext					= nullptr;
		createInfo.flags					= 0;
		createInfo.attachmentCount			= attachmentsDesc.size();
		createInfo.pAttachments				= attachmentsDesc.data();
		createInfo.subpassCount				= subpassesDesc.size();
		createInfo.pSubpasses				= subpassesDesc.data();
		createInfo.dependencyCount			= dependenciesDesc.size();
		createInfo.pDependencies			= dependenciesDesc.data();

		VK_CHECK_RESULT(vkCreateRenderPass(m_pDevice->GetHandle(), &createInfo, nullptr, &m_RenderPass), "Failed to create renderpass");
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		vkDestroyRenderPass(m_pDevice->GetHandle(), m_RenderPass, nullptr);
	}

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	VulkanFrameBuffer::VulkanFrameBuffer(
		const VulkanDevice* pDevice, 
		uint32_t width, 
		uint32_t height, 
		VkImage imageHandle, 
		const VulkanRenderPass* pRenderPass, 
		const std::vector<VkImageView>& attachments
	)
		: RHIFrameBuffer(width, height, 0)
		, m_pDevice(pDevice)
		, m_FrameBufferHandle(VK_NULL_HANDLE)
		, m_pRenderPass(pRenderPass)
		, m_ImageHandle(imageHandle)
		, m_Views(attachments)
		, m_FrameAvailableSemaphore(pDevice)
	{
		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.pNext			= nullptr;
		createInfo.flags			= 0;
		createInfo.renderPass		= pRenderPass->GetHandle();
		createInfo.attachmentCount	= attachments.size();
		createInfo.pAttachments		= attachments.data();
		createInfo.width			= m_Width;
		createInfo.height			= m_Height;
		createInfo.layers			= 1;

		VK_CHECK_RESULT(vkCreateFramebuffer(pDevice->GetHandle(), &createInfo, nullptr, &m_FrameBufferHandle), "Failed to create a framebuffer");
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
		vkDestroyFramebuffer(m_pDevice->GetHandle(), m_FrameBufferHandle, nullptr);
		for (auto view : m_Views)
			vkDestroyImageView(m_pDevice->GetHandle(), view, nullptr);
	}

}
