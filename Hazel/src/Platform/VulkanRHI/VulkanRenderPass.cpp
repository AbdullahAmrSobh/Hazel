#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanRenderPass.h"

namespace Hazel {

	VulkanRenderPass::VulkanRenderPass(const VulkanDevice* pDevice, const VulkanRenderPassDesc& desc)
		: m_pDevice(pDevice), m_RenderPassHandle(VK_NULL_HANDLE)
	{
		Init(desc);
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		vkDestroyRenderPass(m_pDevice->GetHandle(), m_RenderPassHandle, nullptr);
	}

	void VulkanRenderPass::Init(const VulkanRenderPassDesc& desc)
	{
		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.pNext				= nullptr;
		createInfo.flags				= 0;
		createInfo.attachmentCount		= desc.AttachmentDesc.size();
		createInfo.pAttachments			= desc.AttachmentDesc.size() == 0 ? nullptr :  desc.AttachmentDesc.data();
		createInfo.subpassCount			= desc.SubPassDesc.size();
		createInfo.pSubpasses			= desc.SubPassDesc.size() == 0 ? nullptr : desc.SubPassDesc.data();
		createInfo.dependencyCount		= desc.SubPassDependencies.size();
		createInfo.pDependencies		= desc.SubPassDependencies.size() == 0 ? nullptr : desc.SubPassDependencies.data();

		VK_ASSERT(vkCreateRenderPass(m_pDevice->GetHandle(), &createInfo, nullptr, &m_RenderPassHandle), "Failed to create a VkRenderPass");
	}

}
