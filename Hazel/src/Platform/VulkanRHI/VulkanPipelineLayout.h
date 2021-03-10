#pragma once
#include "Hazel/RHI/RHIPipelineLayout.h"

namespace Hazel {

	class VulkanDevice;
	struct VulkanPipelineLayoutDesc
	{
		VulkanPipelineLayoutDesc(const RHIPipelineLayoutDesc& desc)
		{
			CreateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			CreateInfo.pNext					= nullptr;
			CreateInfo.flags					= 0;
			CreateInfo.setLayoutCount			= 0;
			CreateInfo.pSetLayouts				= nullptr;
			CreateInfo.pushConstantRangeCount	= 0;
			CreateInfo.pPushConstantRanges		= nullptr;
		}

		inline auto Read() const { return &CreateInfo; }

		VkPipelineLayoutCreateInfo CreateInfo = {};
	};

	class VulkanPipelineLayout : public RHIPipelineLayout
	{
	public:
		VulkanPipelineLayout(const VulkanDevice* pDevice, const VulkanPipelineLayoutDesc& desc);
		~VulkanPipelineLayout();

		inline VkPipelineLayout GetHandle() const { return m_LayoutHandle; }

	private:
		const VulkanDevice* m_pDevice;
		VkPipelineLayout m_LayoutHandle;
	};
}
