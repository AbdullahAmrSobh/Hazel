#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanPipelineLayout.h"

namespace Hazel {
	
	VulkanPipelineLayout::VulkanPipelineLayout(const VulkanDevice* pDevice, const VulkanPipelineLayoutDesc& desc)
		: m_pDevice(pDevice), m_LayoutHandle(VK_NULL_HANDLE)
	{
		VK_ASSERT(vkCreatePipelineLayout(m_pDevice->GetHandle(), desc.Read(), nullptr, &m_LayoutHandle), "Failed to create a pipeline handle");
	}

	VulkanPipelineLayout::~VulkanPipelineLayout()
	{
		vkDestroyPipelineLayout(m_pDevice->GetHandle(), m_LayoutHandle, nullptr);
	}

}
