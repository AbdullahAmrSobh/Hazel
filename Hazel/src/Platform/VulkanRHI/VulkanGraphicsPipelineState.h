#pragma once
#include "Hazel/RHI/RHIPipelineState.h"

#include "Platform/VulkanRHI/VulkanShader.h"
#include "Platform/VulkanRHI/VulkanPipelineLayout.h"
#include "Platform/VulkanRHI/VulkanRenderPass.h"

namespace Hazel {
	
	class VulkanDevice;
	class VulkanPipelineLayout;
	class VulkanRenderPass;

	struct VulkanPipelineShaderStagesDesc
	{
		VulkanPipelineShaderStagesDesc(
			const Ref<VulkanShader> pInVertexShader,
			const Ref<VulkanShader> pInFragmentShader
		)
			: pVertexShader(pInVertexShader)
 			, pFragmentShader(pInFragmentShader)
		{
			VkPipelineShaderStageCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.pNext = nullptr;

			createInfo.flags = 0;
			createInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			createInfo.module = pVertexShader->GetHandle();
			createInfo.pName = "main";
			createInfo.pSpecializationInfo = nullptr;

			CreateInfos.push_back(createInfo);
			createInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			createInfo.module = pFragmentShader->GetHandle();
			CreateInfos.push_back(createInfo);
		}

		const Ref<VulkanShader> pVertexShader;
		const Ref<VulkanShader> pFragmentShader;

		std::vector<VkPipelineShaderStageCreateInfo> CreateInfos;

		inline auto Read(uint32_t& count) const { count = CreateInfos.size();  return CreateInfos.data(); }
	};

	struct VulkanPipelineVertexInputStateDesc
	{		
		VulkanPipelineVertexInputStateDesc()
		{
			CreateInfo = {};
			CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			CreateInfo.pNext = nullptr;
			CreateInfo.flags = 0;
			CreateInfo.vertexBindingDescriptionCount = VertexBindingDescriptions.size();
			CreateInfo.pVertexBindingDescriptions = VertexBindingDescriptions.data();
			CreateInfo.vertexAttributeDescriptionCount = VertexAttributeDescriptions.size();
			CreateInfo.pVertexAttributeDescriptions = VertexAttributeDescriptions.data();
		}

		VulkanPipelineVertexInputStateDesc(RHIVertexInputStateDesc& vertexInputStateDesc)
		{
			uint32_t location = 0;
			for (auto& attribute : vertexInputStateDesc.Attributes)
			{
				VkVertexInputAttributeDescription attributeDesc = {};
				attributeDesc.location = location;
				attributeDesc.binding = 0;
				attributeDesc.format = ConvertToVulkanFormat(attribute.Format);
				attributeDesc.offset = attribute.Offset;
			
				VertexAttributeDescriptions.push_back(attributeDesc);
			}

			VkVertexInputBindingDescription bindDesc = {};
			bindDesc.binding = 0;
			bindDesc.stride = vertexInputStateDesc.Stride;
			bindDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			VertexBindingDescriptions.push_back(bindDesc);

			CreateInfo = {};
			CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			CreateInfo.pNext = nullptr;
			CreateInfo.flags = 0;
			CreateInfo.vertexBindingDescriptionCount = VertexBindingDescriptions.size();
			CreateInfo.pVertexBindingDescriptions = VertexBindingDescriptions.data();
			CreateInfo.vertexAttributeDescriptionCount = VertexAttributeDescriptions.size();
			CreateInfo.pVertexAttributeDescriptions = VertexAttributeDescriptions.data();
		}
		inline auto Read() const { return &CreateInfo; }

		std::vector<VkVertexInputAttributeDescription>	VertexAttributeDescriptions;
		std::vector<VkVertexInputBindingDescription>	VertexBindingDescriptions;
		VkPipelineVertexInputStateCreateInfo			CreateInfo;
	};

	struct VulkanPipelineInputAssemblyStateDesc
	{
		VulkanPipelineInputAssemblyStateDesc()
		{
			CreateInfo = {};
			CreateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			CreateInfo.pNext					= nullptr;
			CreateInfo.flags					= 0;
			CreateInfo.topology					= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			CreateInfo.primitiveRestartEnable	= VK_FALSE;
		}
		inline auto Read() const { return &CreateInfo; }

		VkPipelineInputAssemblyStateCreateInfo CreateInfo;
	};

	struct VulkanPipelineTessellationStateDesc
	{
		VulkanPipelineTessellationStateDesc()
			: Enable(false)
 		{
			CreateInfo = {};
		}
		VulkanPipelineTessellationStateDesc(uint32_t patchControlPoints)
			: Enable(true)
		{
			CreateInfo = {};
			CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
			CreateInfo.pNext = nullptr;
			CreateInfo.flags = 0;
			CreateInfo.patchControlPoints = patchControlPoints;
		}
		inline auto Read() const { return Enable? &CreateInfo : nullptr; }

		bool								  Enable;
		VkPipelineTessellationStateCreateInfo CreateInfo;
	};

	struct VulkanPipelineViewportStateDesc
	{
		VulkanPipelineViewportStateDesc(const RHIViewport& viewportDesc, const RHIScissor& scissorDesc)
		{
			VkViewport viewport;
			viewport.x			= viewportDesc.OffsetX;
			viewport.y			= viewportDesc.OffsetY;
			viewport.width		= viewportDesc.Width;
			viewport.height		= viewportDesc.Height;
			viewport.minDepth	= viewportDesc.Min;
			viewport.maxDepth	= viewportDesc.Max;
			Viewports.push_back(viewport);

			VkRect2D scissor;
			scissor.offset.x		= viewportDesc.OffsetX;
			scissor.offset.y		= viewportDesc.OffsetY;
			scissor.extent.width	= viewportDesc.Width;
			scissor.extent.height	= viewportDesc.Height;
			Scissors.push_back(scissor);

			CreateInfo = {};
			CreateInfo.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			CreateInfo.pNext			= nullptr;
			CreateInfo.flags			= 0;
			CreateInfo.viewportCount	= Viewports.size();
			CreateInfo.pViewports		= Viewports.data();
			CreateInfo.scissorCount		= Scissors.size();
			CreateInfo.pScissors		= Scissors.data();
		}
		inline auto Read() const { return &CreateInfo; }

		std::vector<VkViewport> Viewports;
		std::vector<VkRect2D>	Scissors;

		VkPipelineViewportStateCreateInfo CreateInfo;

	};

	struct VulkanPipelineRasterizationStateDesc
	{
		VulkanPipelineRasterizationStateDesc()
		{
			CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			CreateInfo.pNext = nullptr;
			CreateInfo.flags = 0;
			CreateInfo.depthClampEnable = VK_FALSE;
			CreateInfo.rasterizerDiscardEnable = VK_FALSE;
			CreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
			CreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
			CreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
			CreateInfo.depthBiasEnable = VK_FALSE;
			CreateInfo.depthBiasConstantFactor = 0.0f;
			CreateInfo.depthBiasClamp = 0.0f;
			CreateInfo.depthBiasSlopeFactor = 0.0f;
			CreateInfo.lineWidth = 1.0f;
		}
		inline auto Read() const { return &CreateInfo; }

		VkPipelineRasterizationStateCreateInfo CreateInfo;
	};

	struct VulkanPipelineMultisampleStateDesc
	{
		VulkanPipelineMultisampleStateDesc()
		{
			CreateInfo = {};
			CreateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			CreateInfo.pNext					= nullptr;
			CreateInfo.flags					= 0;
			CreateInfo.rasterizationSamples		= VK_SAMPLE_COUNT_1_BIT;
			CreateInfo.sampleShadingEnable		= VK_FALSE;
			CreateInfo.minSampleShading			= 1.0f;
			CreateInfo.pSampleMask				= nullptr;
			CreateInfo.alphaToCoverageEnable	= VK_FALSE;
			CreateInfo.alphaToOneEnable			= VK_FALSE;
		}
		inline auto Read() const { return &CreateInfo; }

		VkPipelineMultisampleStateCreateInfo CreateInfo;
	};

	struct VulkanPipelineDepthStencilStateDesc
	{
		VulkanPipelineDepthStencilStateDesc()
			: Enable(false)
		{
			CreateInfo = {};
		}
		inline auto Read() const { return Enable? &CreateInfo : nullptr; }

		bool Enable;
		VkPipelineDepthStencilStateCreateInfo CreateInfo;
	};

	struct VulkanPipelineColorBlendStateDesc
	{
		VulkanPipelineColorBlendStateDesc()
		{

			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
			ColorBlendAttachmentStates.push_back(colorBlendAttachment);

			CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			CreateInfo.pNext = nullptr;
			CreateInfo.flags = 0;
			CreateInfo.logicOpEnable = VK_FALSE;
			CreateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
			CreateInfo.attachmentCount = ColorBlendAttachmentStates.size();
			CreateInfo.pAttachments = ColorBlendAttachmentStates.data();
			CreateInfo.blendConstants[0] = 0.0f; // Optional
			CreateInfo.blendConstants[1] = 0.0f; // Optional
			CreateInfo.blendConstants[2] = 0.0f; // Optional
			CreateInfo.blendConstants[3] = 0.0f; // Optional
		}
		inline auto Read() const { return &CreateInfo; }

		std::vector<VkPipelineColorBlendAttachmentState> ColorBlendAttachmentStates;
		VkPipelineColorBlendStateCreateInfo CreateInfo;
	};

	struct VulkanPipelineDynamicStateDesc
	{
		VulkanPipelineDynamicStateDesc()
		{
			DynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
			DynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

			CreateInfo = {};
			CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			CreateInfo.flags = 0;
			CreateInfo.pNext = nullptr;
			CreateInfo.dynamicStateCount = DynamicStates.size();
			CreateInfo.pDynamicStates = DynamicStates.data();
		}
		inline auto Read() const { return &CreateInfo; }

		std::vector<VkDynamicState> DynamicStates;
		VkPipelineDynamicStateCreateInfo CreateInfo;
	};

	struct VulkanGraphicsPipelineStateDesc
	{
		VulkanGraphicsPipelineStateDesc(const RHIGraphicsPipelineStateDesc& desc, const VulkanPipelineLayout* pInLayout, const VulkanRenderPass* pInRenderPass)
			: ShaderStateDesc(
				std::dynamic_pointer_cast<VulkanShader>(desc.pVertexShader),
				std::dynamic_pointer_cast<VulkanShader>(desc.pPixelShader)
			)
			, VertexInputStateDesc()
			, InputAssemblyStateDesc()
			, TessellationStateDesc()
			, ViewportStateDesc(desc.pViewportStateDesc->Viewport, desc.pViewportStateDesc->Scissor)
			, RasterizationStateDesc()
			, MultisampleStateDesc()
			, DepthStencilStateDesc()
			, ColorBlendStateDesc()
			, DynamicStateDesc()
			, pLayout(pInLayout)
			, pRenderPass(pInRenderPass)
		{
			CreateInfo.sType				= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			CreateInfo.pNext				= nullptr;
			CreateInfo.flags				= 0;
			CreateInfo.pStages				= ShaderStateDesc.Read(CreateInfo.stageCount);
			CreateInfo.pVertexInputState	= VertexInputStateDesc.Read();
			CreateInfo.pInputAssemblyState	= InputAssemblyStateDesc.Read();
			CreateInfo.pTessellationState	= TessellationStateDesc.Read();
			CreateInfo.pViewportState		= ViewportStateDesc.Read();
			CreateInfo.pRasterizationState	= RasterizationStateDesc.Read();
			CreateInfo.pMultisampleState	= MultisampleStateDesc.Read();
			CreateInfo.pDepthStencilState	= DepthStencilStateDesc.Read();
			CreateInfo.pColorBlendState		= ColorBlendStateDesc.Read();
			CreateInfo.pDynamicState		= DynamicStateDesc.Read();
			CreateInfo.layout				= pLayout->GetHandle();
			CreateInfo.renderPass			= pRenderPass->GetHandle();
			CreateInfo.subpass				= 0;
			CreateInfo.basePipelineHandle	= VK_NULL_HANDLE;
			CreateInfo.basePipelineIndex	= 0;
		}
		inline auto Read() const { return &CreateInfo; }

		VulkanPipelineShaderStagesDesc			ShaderStateDesc;
		VulkanPipelineVertexInputStateDesc		VertexInputStateDesc;
		VulkanPipelineInputAssemblyStateDesc	InputAssemblyStateDesc;
		VulkanPipelineTessellationStateDesc		TessellationStateDesc;
		VulkanPipelineViewportStateDesc			ViewportStateDesc;
		VulkanPipelineRasterizationStateDesc	RasterizationStateDesc;
		VulkanPipelineMultisampleStateDesc		MultisampleStateDesc;
		VulkanPipelineDepthStencilStateDesc		DepthStencilStateDesc;
		VulkanPipelineColorBlendStateDesc		ColorBlendStateDesc;
		VulkanPipelineDynamicStateDesc			DynamicStateDesc;
		const VulkanPipelineLayout*				pLayout;
		const VulkanRenderPass*					pRenderPass;

		VkGraphicsPipelineCreateInfo			CreateInfo;

	};

	class VulkanGraphicsPipelineState : public RHIGraphicsPipelineState
	{
	public:
		VulkanGraphicsPipelineState(const VulkanDevice* pDevice, const VulkanGraphicsPipelineStateDesc& desc);
		~VulkanGraphicsPipelineState();
		

		inline VkPipeline GetHandle() const { return m_PipelineState; }
	private:
		const VulkanDevice*		m_pDevice;
		VkPipeline				m_PipelineState;
		// VulkanPipelineLayout*	m_pPipelineLayout;
		// VulkanRenderPass*		m_pRenderPass;
	};

}
