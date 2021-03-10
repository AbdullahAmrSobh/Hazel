#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanGraphicsPipelineState.h"

namespace Hazel {
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VulkanGraphicsPipelineState::VulkanGraphicsPipelineState(const VulkanDevice* pDevice, const VulkanGraphicsPipelineStateDesc& desc)
		: m_pDevice(pDevice), m_PipelineState(VK_NULL_HANDLE)
	{
		VK_ASSERT(vkCreateGraphicsPipelines(m_pDevice->GetHandle(), VK_NULL_HANDLE, 1, desc.Read(), nullptr, &m_PipelineState), "Failed to create Graphics VkPipeline");
	}

	VulkanGraphicsPipelineState::~VulkanGraphicsPipelineState()
	{
		vkDestroyPipeline(m_pDevice->GetHandle(), m_PipelineState, nullptr);
	}

}
