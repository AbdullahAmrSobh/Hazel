#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanRenderPass.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"


namespace Hazel {

	VulkanFrameBuffer::VulkanFrameBuffer(const VulkanDevice* pDevice, const VulkanFrameBufferDesc& desc)
		: m_pDevice(pDevice), m_pRenderPass(desc.pRenderPass), m_FrameBufferHandle(VK_NULL_HANDLE), m_ImageHandle(desc.ImageHandle)
	{
		Init(desc);
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
		vkDestroyFramebuffer(m_pDevice->GetHandle(), m_FrameBufferHandle, nullptr);
		
		for(auto& image : m_AttachmentViews)
			vkDestroyImageView(m_pDevice->GetHandle(), image, nullptr);

		vkDestroyImage(m_pDevice->GetHandle(), m_ImageHandle, nullptr);
	}

	void VulkanFrameBuffer::Init(const VulkanFrameBufferDesc& desc)
	{
		VkFramebufferCreateInfo createInfo	= {};
		createInfo.sType					= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.pNext					= nullptr;
		createInfo.flags					= 0;
		createInfo.renderPass				= m_pRenderPass->GetHandle();
		createInfo.attachmentCount			= desc.Views.size();
		createInfo.pAttachments				= desc.Views.data();
		createInfo.width					= desc.Width;
		createInfo.height					= desc.Height;
		createInfo.layers					= desc.Layers;

		VK_ASSERT(vkCreateFramebuffer(m_pDevice->GetHandle(), &createInfo, nullptr, &m_FrameBufferHandle), "Failed to create VulkanFrameBuffer");
	}

}
