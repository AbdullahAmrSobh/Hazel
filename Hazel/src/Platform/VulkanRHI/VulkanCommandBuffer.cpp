#pragma once
#include "hzpch.h"

#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanCommandBuffer.h"

#include "Platform/VulkanRHI/VulkanFrameBuffer.h"
#include "Platform/VulkanRHI/VulkanRenderPass.h"
#include "Platform/VulkanRHI/VulkanPipelineLayout.h"
#include "Platform/VulkanRHI/VulkanGraphicsPipelineState.h"
#include "Platform/VulkanRHI/VulkanSwapChain.h"


namespace Hazel {
	VulkanCommandsAllocator::VulkanCommandsAllocator(const VulkanDevice* pDevice, uint32_t queueFamilyIndex)
		: m_pDevice(pDevice), m_CommandPooLHandle(VK_NULL_HANDLE)
	{
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.queueFamilyIndex = queueFamilyIndex;

		VK_ASSERT(vkCreateCommandPool(m_pDevice->GetHandle(), &createInfo, nullptr, &m_CommandPooLHandle), "Failed to Create a CommandPool");
	}

	VulkanCommandsAllocator::~VulkanCommandsAllocator()
	{
		vkDestroyCommandPool(m_pDevice->GetHandle(), m_CommandPooLHandle, nullptr);
	}

	std::vector<VulkanCommandBuffer> VulkanCommandsAllocator::AllocateCommandBuffers(uint32_t count, VkCommandBufferLevel level)
	{
		std::vector<VkCommandBuffer> commandBuffer;
		commandBuffer.resize(count);

		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.pNext = nullptr;
		allocateInfo.commandPool = m_CommandPooLHandle;
		allocateInfo.level = level;
		allocateInfo.commandBufferCount = count;

		VK_ASSERT(vkAllocateCommandBuffers(m_pDevice->GetHandle(), &allocateInfo, commandBuffer.data()), "Failed to allocate command buffer");

		std::vector<VulkanCommandBuffer> cmds;
		for (auto cmd : commandBuffer)
			cmds.push_back(VulkanCommandBuffer(m_pDevice, cmd));
		return cmds;
	}

	void VulkanCommandsAllocator::FreeCommandBuffers(std::vector<VkCommandBuffer> commandBuffers)
	{
		vkFreeCommandBuffers(m_pDevice->GetHandle(), m_CommandPooLHandle, commandBuffers.size(), commandBuffers.data());
	}

	VulkanCommandBuffer::VulkanCommandBuffer(const VulkanDevice* pDevice, VkCommandBuffer bufferHandle)
		: m_pDevice(pDevice), m_CommandBufferHandle(bufferHandle)
	{
	}

	void VulkanCommandBuffer::RecordVulkanDebugCommands(VulkanRenderPass* pRenderPass, VulkanFrameBuffer* pTargetFrameBuffer, VulkanGraphicsPipelineState* pPso)
	{
		VkCommandBufferBeginInfo beginInfo	= {};
		beginInfo.sType						= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext						= nullptr;
		beginInfo.flags						= 0;
		beginInfo.pInheritanceInfo			= nullptr;
		VK_ASSERT(vkBeginCommandBuffer(m_CommandBufferHandle, &beginInfo), "Failed to begin command buffer recording");

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType				 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass			 = pRenderPass->GetHandle();
		renderPassInfo.framebuffer			 = pTargetFrameBuffer->GetHandle();
		renderPassInfo.renderArea.offset	 = { 0, 0 };
		renderPassInfo.renderArea.extent	 = { pTargetFrameBuffer->Width(), pTargetFrameBuffer->Height() };

		VkClearValue clearColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues    = &clearColor;

		vkCmdBeginRenderPass(m_CommandBufferHandle, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(m_CommandBufferHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, pPso->GetHandle());

		vkCmdDraw(m_CommandBufferHandle, 3, 1, 0, 0);

		vkCmdEndRenderPass(m_CommandBufferHandle);

		VK_ASSERT(vkEndCommandBuffer(m_CommandBufferHandle), "Failed to end command buffer recording");
	}

	void VulkanCommandBuffer::RecordDebugCommands(RHIFrameBuffer* pRenderTarget, RHIGraphicsPipelineState* pPso)
	{
		RecordVulkanDebugCommands(
			reinterpret_cast<VulkanFrameBuffer*>(pRenderTarget)->GetRenderPass(),
			reinterpret_cast<VulkanFrameBuffer*>(pRenderTarget),
			reinterpret_cast<VulkanGraphicsPipelineState*>(pPso)
		);
	}
}
