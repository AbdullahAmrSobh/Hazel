#pragma once

#include "Hazel/RHI/RHIShader.h"
#include "Hazel/RHI/RHIResources.h"
#include "Hazel/RHI/RHIPipelineState.h"

#include <vulkan/vulkan.h>

namespace Hazel {

	// Initializers
	
	class VulkanShader;

	class VulkanPipelineShaderStagesInitializer
	{
	public:

		VulkanPipelineShaderStagesInitializer(const RHIShader* pVS, const RHIShader* pFS);

		inline void WriteTo(uint32_t* pCount, const VkPipelineShaderStageCreateInfo** ppStages) 
		{ 
			*pCount = CreateInfos.size();
			*ppStages = CreateInfos.data(); 
		}

	private:

		std::vector<VkPipelineShaderStageCreateInfo> CreateInfos;
	};

	class VulkanPipelineVertexInputStateInitializer
	{
	public:

		VulkanPipelineVertexInputStateInitializer();

		VulkanPipelineVertexInputStateInitializer(const RHIVertexBufferLayout* pLayout);

		inline void WriteTo(const VkPipelineVertexInputStateCreateInfo** ppState)
		{
			*ppState = &CreateInfo;
		}

	private:
		std::vector<VkVertexInputAttributeDescription>	VertexAttributeDescriptions;
		std::vector<VkVertexInputBindingDescription>	VertexBindingDescriptions;
		VkPipelineVertexInputStateCreateInfo			CreateInfo;
	
	};

	class VulkanPipelineInputAssemblyStateInitializer
	{
	public:

		VulkanPipelineInputAssemblyStateInitializer();

		inline void WriteTo(const VkPipelineInputAssemblyStateCreateInfo** ppState) 
		{ 
			*ppState = &CreateInfo; 
		}

	private:

		VkPipelineInputAssemblyStateCreateInfo CreateInfo;
	};

	class VulkanPipelineTessellationStateInitializer
	{
	public:

		VulkanPipelineTessellationStateInitializer();

		VulkanPipelineTessellationStateInitializer(uint32_t patchControlPoints);

		inline void WriteTo(const VkPipelineTessellationStateCreateInfo** ppState)
		{ 
			if (Enable)
				*ppState = &CreateInfo;
			else
				*ppState = nullptr;
		}

	private:
		bool								  Enable;
		VkPipelineTessellationStateCreateInfo CreateInfo;

	};

	class VulkanPipelineViewportStateInitializer
	{
	public:
		VulkanPipelineViewportStateInitializer(const RHIViewport& viewportDesc, const RHIReact2D& scissorDesc);
		
		inline void WriteTo(const VkPipelineViewportStateCreateInfo** ppState) 
		{ 
			*ppState = &CreateInfo; 
		}

	private:
		std::vector<VkViewport> Viewports;
		std::vector<VkRect2D>	Scissors;

		VkPipelineViewportStateCreateInfo CreateInfo;

	};

	class VulkanPipelineRasterizationStateInitializer
	{
	public:
		
		VulkanPipelineRasterizationStateInitializer();
		
		inline void WriteTo(const VkPipelineRasterizationStateCreateInfo** ppState)
		{
			*ppState = &CreateInfo;
		}

	private:

		VkPipelineRasterizationStateCreateInfo CreateInfo;
	};

	class VulkanPipelineMultisampleStateInitializer
	{
	public:
		VulkanPipelineMultisampleStateInitializer();

		inline void WriteTo(const VkPipelineMultisampleStateCreateInfo** ppState)
		{ 
			*ppState = &CreateInfo;
		}

	private:

		VkPipelineMultisampleStateCreateInfo CreateInfo;
	};

	class VulkanPipelineDepthStencilStateInitializer
	{
	public:

		VulkanPipelineDepthStencilStateInitializer();

		inline void WriteTo(const VkPipelineDepthStencilStateCreateInfo** ppState)
		{ 
			if (Enable)
				*ppState = &CreateInfo;
			else
				*ppState = nullptr;
		}

	private:

		bool Enable;
		VkPipelineDepthStencilStateCreateInfo CreateInfo;
	};

	class VulkanPipelineColorBlendStateInitializer
	{
	public:

		VulkanPipelineColorBlendStateInitializer();
		
		inline void WriteTo(const VkPipelineColorBlendStateCreateInfo** ppState)
		{ 
			*ppState = &CreateInfo;
		}

	private:
		std::vector<VkPipelineColorBlendAttachmentState> ColorBlendAttachmentStates;
		VkPipelineColorBlendStateCreateInfo CreateInfo;

	};

	class VulkanPipelineDynamicStateInitializer
	{
	public:

		VulkanPipelineDynamicStateInitializer();

		inline void WriteTo(const VkPipelineDynamicStateCreateInfo** ppState)
		{ 
			*ppState = &CreateInfo; 
		}

	public:

		std::vector<VkDynamicState> DynamicStates;
		VkPipelineDynamicStateCreateInfo CreateInfo;

	};

	//  Pipeline Layout 
	class VulkanPipelineLayout : public RHIPipelineLayout
	{
	public:

		VulkanPipelineLayout(const VulkanDevice* pDevice);
		VulkanPipelineLayout(
			const VulkanDevice* pDevice, 
			uint32_t layoutCount, 
			const VkDescriptorSetLayout* pLayouts, 
			uint32_t constantsCount = 0, 
			const VkPushConstantRange* pushConstants = nullptr
		);

		~VulkanPipelineLayout();

		inline VkPipelineLayout GetHandle() const { return m_LayoutHandle; }

	private:
		const VulkanDevice* m_pDevice;
		VkPipelineLayout m_LayoutHandle;
	};

	// Pipeline States
	class VulkanGraphicsPipelineState : public RHIGraphicsPipelineState
	{
	public:
		VulkanGraphicsPipelineState(const VulkanDevice* pDevice, const VkGraphicsPipelineCreateInfo& createInfo);
		~VulkanGraphicsPipelineState();

		inline VkPipeline GetHandle() const { return m_PipelineHandle; }

	private:
		const VulkanDevice* m_pDevice;
		VkPipeline			m_PipelineHandle;
	};


}
