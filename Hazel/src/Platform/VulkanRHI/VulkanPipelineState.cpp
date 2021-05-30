#include "hzpch.h"

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanShader.h"
#include "Platform/VulkanRHI/VulkanPipelineState.h"
#include "Platform/VulkanRHI/VulkanDescriptors.h"
#include "Platform/VulkanRHI/VulkanSwapChain.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"
#include "Platform/VulkanRHI/VulkanRHI.h"

namespace Hazel {


	RHIPipelineLayout* VulkanRHI::CreatePipelineLayout(const RHIPipelineLayoutDesc& desc)
	{
		std::vector<VkDescriptorSetLayout> layouts = {};
		layouts.reserve(desc.DescriptorsLayouts.size());

		for (auto& layout : desc.DescriptorsLayouts) layouts.push_back(m_pDescriptorsLayoutManager->GetOrCreate(layout));

		return new VulkanPipelineLayout(m_pDevice, layouts.size(), layouts.data());
	}

	RHIGraphicsPipelineState* VulkanRHI::CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& desc)
	{
		RHIViewport viewportDesc = {};
		viewportDesc.Width = m_pWindow->GetWidth();
		viewportDesc.Height = m_pWindow->GetHeight();

		RHIReact2D reactDesc = {};
		reactDesc.Width = m_pWindow->GetWidth();
		reactDesc.Height = m_pWindow->GetHeight();

		VulkanPipelineShaderStagesInitializer		shaderStage(desc.pVS, desc.pPS);
		VulkanPipelineVertexInputStateInitializer	vertexInput(desc.pVertexLayout);
		VulkanPipelineInputAssemblyStateInitializer inputAssembly;
		VulkanPipelineTessellationStateInitializer	tessellation;
		VulkanPipelineViewportStateInitializer		viewport(viewportDesc, reactDesc);
		VulkanPipelineRasterizationStateInitializer rasterization;
		VulkanPipelineMultisampleStateInitializer	multisample;
		VulkanPipelineDepthStencilStateInitializer	depthStencil;
		VulkanPipelineColorBlendStateInitializer	colorBlend;
		VulkanPipelineDynamicStateInitializer		dynamic;

		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;

		shaderStage.WriteTo(&createInfo.stageCount, &createInfo.pStages);
		vertexInput.WriteTo(&createInfo.pVertexInputState);
		inputAssembly.WriteTo(&createInfo.pInputAssemblyState);
		tessellation.WriteTo(&createInfo.pTessellationState);
		viewport.WriteTo(&createInfo.pViewportState);
		rasterization.WriteTo(&createInfo.pRasterizationState);
		multisample.WriteTo(&createInfo.pMultisampleState);
		depthStencil.WriteTo(&createInfo.pDepthStencilState);
		colorBlend.WriteTo(&createInfo.pColorBlendState);
		dynamic.WriteTo(&createInfo.pDynamicState);

		VulkanPipelineLayout* pLayout = reinterpret_cast<VulkanPipelineLayout*>(desc.pLayout);

		createInfo.layout = pLayout->GetHandle();
		createInfo.renderPass = m_pSwapChain->GetRenderPass()->GetHandle();
		createInfo.subpass = 0;
		createInfo.basePipelineHandle = VK_NULL_HANDLE;
		createInfo.basePipelineIndex = 0;

		return new VulkanGraphicsPipelineState(m_pDevice, createInfo);
	}


	VulkanPipelineShaderStagesInitializer::VulkanPipelineShaderStagesInitializer(const RHIShader* pVS, const RHIShader* pFS)
	{
		const VulkanShader* pVertexShader = reinterpret_cast<const VulkanShader*>(pVS);
		const VulkanShader* pFragmentShader = reinterpret_cast<const VulkanShader*>(pFS);

		VkPipelineShaderStageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		createInfo.module = pVertexShader->GetHandle();
		createInfo.pName = pVertexShader->GetName().c_str();
		createInfo.pSpecializationInfo = nullptr;

		CreateInfos.push_back(createInfo);

		createInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		createInfo.module = pFragmentShader->GetHandle();
		createInfo.pName = pFragmentShader->GetName().c_str();
		CreateInfos.push_back(createInfo);
	}

	VulkanPipelineVertexInputStateInitializer::VulkanPipelineVertexInputStateInitializer()
	{
		CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		CreateInfo.pNext = nullptr;
		CreateInfo.flags = 0;
		CreateInfo.vertexBindingDescriptionCount = 0;
		CreateInfo.pVertexBindingDescriptions = nullptr;
		CreateInfo.vertexAttributeDescriptionCount = 0;
		CreateInfo.pVertexAttributeDescriptions = nullptr;
	}

	VulkanPipelineVertexInputStateInitializer::VulkanPipelineVertexInputStateInitializer(const RHIVertexBufferLayout* pLayout)
	{
		uint32_t index = 0;

		VkVertexInputBindingDescription bindingDesc = {};
		bindingDesc.binding = 0;
		bindingDesc.stride = 0;
		bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		for (auto attribute : pLayout->Attributes)
		{
			VkVertexInputAttributeDescription attributeDesc = {};
			attributeDesc.location = index;
			attributeDesc.binding = 0;
			attributeDesc.format = VulkanUtils::GetFromat(attribute.Format);
			attributeDesc.offset = bindingDesc.stride;

			bindingDesc.stride += GetFormatSize(attribute.Format);

			index++;

			VertexAttributeDescriptions.push_back(attributeDesc);
		}

		VertexBindingDescriptions.push_back(bindingDesc);

		CreateInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		CreateInfo.pNext							= nullptr;
		CreateInfo.flags							= 0;
		CreateInfo.vertexBindingDescriptionCount	= VertexBindingDescriptions.size();
		CreateInfo.pVertexBindingDescriptions		= VertexBindingDescriptions.data();
		CreateInfo.vertexAttributeDescriptionCount	= VertexAttributeDescriptions.size();
		CreateInfo.pVertexAttributeDescriptions		= VertexAttributeDescriptions.data();
	}

	VulkanPipelineInputAssemblyStateInitializer::VulkanPipelineInputAssemblyStateInitializer()
	{
		CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		CreateInfo.pNext = nullptr;
		CreateInfo.flags = 0;
		CreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		CreateInfo.primitiveRestartEnable = VK_FALSE;
	}

	VulkanPipelineTessellationStateInitializer::VulkanPipelineTessellationStateInitializer()
		: Enable(false)
	{
		CreateInfo = {};
	}

	VulkanPipelineTessellationStateInitializer::VulkanPipelineTessellationStateInitializer(uint32_t patchControlPoints)
		: Enable(true)
	{
		CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		CreateInfo.pNext = nullptr;
		CreateInfo.flags = 0;
		CreateInfo.patchControlPoints = patchControlPoints;
	}

	VulkanPipelineViewportStateInitializer::VulkanPipelineViewportStateInitializer(const RHIViewport& viewportDesc, const RHIReact2D& scissorDesc)
	{
		VkViewport viewport = {};
		viewport.x = viewportDesc.OffsetX;
		viewport.y = viewportDesc.OffsetY;
		viewport.width = viewportDesc.Width;
		viewport.height = viewportDesc.Height;
		viewport.minDepth = viewportDesc.MinDepth;
		viewport.maxDepth = viewportDesc.MaxDepth;
		Viewports.push_back(viewport);

		VkRect2D scissor = {};
		scissor.offset.x = scissorDesc.OffsetX;
		scissor.offset.y = scissorDesc.OffsetY;
		scissor.extent.width = scissorDesc.Width;
		scissor.extent.height = scissorDesc.Height;
		Scissors.push_back(scissor);

		CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		CreateInfo.pNext = nullptr;
		CreateInfo.flags = 0;
		CreateInfo.viewportCount = Viewports.size();
		CreateInfo.pViewports = Viewports.data();
		CreateInfo.scissorCount = Scissors.size();
		CreateInfo.pScissors = Scissors.data();
	}

	VulkanPipelineRasterizationStateInitializer::VulkanPipelineRasterizationStateInitializer()
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

	VulkanPipelineMultisampleStateInitializer::VulkanPipelineMultisampleStateInitializer()
	{
		CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		CreateInfo.pNext = nullptr;
		CreateInfo.flags = 0;
		CreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		CreateInfo.sampleShadingEnable = VK_FALSE;
		CreateInfo.minSampleShading = 1.0f;
		CreateInfo.pSampleMask = nullptr;
		CreateInfo.alphaToCoverageEnable = VK_FALSE;
		CreateInfo.alphaToOneEnable = VK_FALSE;
	}

	VulkanPipelineDepthStencilStateInitializer::VulkanPipelineDepthStencilStateInitializer()
		: Enable(false)
	{
		CreateInfo = {};
	}

	VulkanPipelineColorBlendStateInitializer::VulkanPipelineColorBlendStateInitializer()
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

	VulkanPipelineDynamicStateInitializer::VulkanPipelineDynamicStateInitializer()
	{
		// DynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
		// DynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

		CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		CreateInfo.flags = 0;
		CreateInfo.pNext = nullptr;
		CreateInfo.dynamicStateCount = DynamicStates.size();
		CreateInfo.pDynamicStates = DynamicStates.data();
	}
	

	// Pipeline state
	VulkanGraphicsPipelineState::VulkanGraphicsPipelineState(const VulkanDevice* pDevice, const VkGraphicsPipelineCreateInfo& createInfo)
		: m_pDevice(pDevice)
		, m_PipelineHandle(VK_NULL_HANDLE)
	{

		VkResult result = vkCreateGraphicsPipelines(m_pDevice->GetHandle(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &m_PipelineHandle);
		VK_CHECK_RESULT(result, "Failed to create a graphics pipeline");
	}

	VulkanGraphicsPipelineState::~VulkanGraphicsPipelineState()
	{
		vkDestroyPipeline(m_pDevice->GetHandle(), m_PipelineHandle, nullptr);
	}


}
