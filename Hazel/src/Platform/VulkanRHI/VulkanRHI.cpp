#include "hzpch.h"

#include <vulkan/vulkan.h>

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanQueue.h"
#include "Platform/VulkanRHI/VulkanMemory.h"
#include "Platform/VulkanRHI/VulkanSwapChain.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"
#include "Platform/VulkanRHI/VulkanShader.h"
#include "Platform/VulkanRHI/VulkanPipelineState.h"
#include "Platform/VulkanRHI/VulkanCommandBuffers.h"

#include "Platform/VulkanRHI/VulkanDescriptors.h"

#include "Platform/VulkanRHI/VulkanResources.h"

#include "Platform/VulkanRHI/VulkanRHI.h"


namespace Hazel
{

	void VulkanRHI::OnInit()
	{
		m_pCommandAllocator = new VulkanCommandAllocator(m_pDevice, &m_pDevice->GetGraphicsQueue());
	}

	void VulkanRHI::OnShutdown()
	{
		delete m_pCommandAllocator;
	}

	void VulkanRHI::OnUpdate()
	{
		m_pSwapChain->Present();
		m_pSwapChain->SwapBuffers();
	}

	RHISwapChain* VulkanRHI::GetSwapChain()
	{
		return reinterpret_cast<RHISwapChain*>(m_pSwapChain);
	}

	RHIShaderCompiler* VulkanRHI::GetShaderCompiler()
	{
		return reinterpret_cast<RHIShaderCompiler*>(m_pShaderCompiler);
	}

	RHIFence* VulkanRHI::CreateFence()
	{
		return new VulkanFence(m_pDevice);
	}

	RHIDescriptorPool* VulkanRHI::CreateDescriptorPool(const std::vector<RHIDescriptorSetLayoutDesc>& layouts, uint32_t maxSets)
	{
		return new VulkanDescriptorPool(m_pDevice, m_pDescriptorsLayoutManager, layouts, maxSets);
	}

	RHIPipelineLayout* VulkanRHI::CreatePipelineLayout(const RHIPipelineLayoutDesc& desc)
	{
		std::vector<VkDescriptorSetLayout> layouts = {};
		layouts.reserve(desc.DescriptorsLayouts.size());

		for (auto& layout : desc.DescriptorsLayouts)
			layouts.push_back(m_pDescriptorsLayoutManager->GetOrCreate(layout));
		
		return new VulkanPipelineLayout(m_pDevice, layouts.size(), layouts.data());
	}

	RHIGraphicsPipelineState* VulkanRHI::CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& desc)
	{
		RHIViewport viewportDesc;
		viewportDesc.Width = m_pWindow->GetWidth();
		viewportDesc.Height = m_pWindow->GetHeight();

		RHIReact2D reactDesc;
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


	RHICommandBuffer* VulkanRHI::AllocateCommandBuffer()
	{
		return new VulkanCommandBuffer(m_pCommandAllocator);
	}

	void VulkanRHI::ExecuteCommandBuffer(RHICommandBuffer* pCommandBuffer, RHIFence* pFence)
	{
		m_pDevice->GetGraphicsQueue().SubmitCommandBuffer(
			reinterpret_cast<VulkanCommandBuffer*>(pCommandBuffer), 
			reinterpret_cast<VulkanFence*>(pFence)
		);
	}

	RHIUniformBuffer* VulkanRHI::CreateUniformBuffer(size_t bufferSize) 
	{
		return new VulkanUniformBuffer(m_pDevice, m_pAllocator, bufferSize);
	}

	RHIStagingBuffer* VulkanRHI::CreateStagingBuffer(size_t bufferSize) 
	{
		return new VulkanStagingBuffer(m_pDevice, m_pAllocator, bufferSize);
	} 
	
	RHIVertexBuffer* VulkanRHI::CreateVertexBuffer(uint32_t vertexCount, uint32_t stride) 
	{
		return new VulkanVertexBuffer(m_pDevice, m_pAllocator, vertexCount, stride);
	}
	
	RHIIndexBuffer* VulkanRHI::CreateIndexBuffer(uint32_t indciesCount, uint32_t stride) 
	{
		return new VulkanIndexBuffer(m_pDevice, m_pAllocator, indciesCount, stride);
	}

}
