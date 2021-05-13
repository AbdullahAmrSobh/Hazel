#include "hzpch.h"

#include <vulkan/vulkan.h>

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanPipelineState.h"

namespace Hazel {

	VulkanPipelineLayout::VulkanPipelineLayout(const VulkanDevice* pDevice)
		: m_pDevice(pDevice), m_LayoutHandle(VK_NULL_HANDLE)
	{
		VkPipelineLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.setLayoutCount = 0;
		createInfo.pSetLayouts = nullptr;
		createInfo.pushConstantRangeCount = 0;
		createInfo.pPushConstantRanges = nullptr;

		VK_CHECK_RESULT(
			vkCreatePipelineLayout(m_pDevice->GetHandle(), &createInfo, nullptr, &m_LayoutHandle),
			"Failed to create a pipline layout"
		);
	}

	VulkanPipelineLayout::VulkanPipelineLayout(const VulkanDevice* pDevice, uint32_t layoutCount, const VkDescriptorSetLayout* pLayouts, uint32_t constantsCount, const VkPushConstantRange* pPushConstants)
		: m_pDevice(pDevice)
		, m_LayoutHandle(VK_NULL_HANDLE)
	{
		VkPipelineLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.setLayoutCount = layoutCount;
		createInfo.pSetLayouts = pLayouts;
		createInfo.pushConstantRangeCount = constantsCount;
		createInfo.pPushConstantRanges = pPushConstants;

		VK_CHECK_RESULT(
			vkCreatePipelineLayout(m_pDevice->GetHandle(), &createInfo, nullptr, &m_LayoutHandle),
			"Failed to create a pipline layout"
		);
	}

	VulkanPipelineLayout::~VulkanPipelineLayout()
	{
		vkDestroyPipelineLayout(m_pDevice->GetHandle(), m_LayoutHandle, nullptr);
	}

}
