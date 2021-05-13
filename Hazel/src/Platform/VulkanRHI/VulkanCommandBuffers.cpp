#include "hzpch.h"

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanQueue.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"
#include "Platform/VulkanRHI/VulkanPipelineState.h"
#include "Platform/VulkanRHI/VulkanResources.h"

#include "Platform/VulkanRHI/VulkanDescriptors.h"

#include "Platform/VulkanRHI/VulkanCommandBuffers.h"


namespace Hazel {

	VulkanCommandAllocator::VulkanCommandAllocator(const VulkanDevice* pDevice, const VulkanQueue* pQueue)
		: m_pDevice(pDevice)
	{

		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.queueFamilyIndex = pQueue->GetFamilyIndex();

		VK_CHECK_RESULT(vkCreateCommandPool(m_pDevice->GetHandle(), &createInfo, nullptr, &m_CommandPoolHandle), 
			"Failed to create a command pool");
	}

	VulkanCommandAllocator::~VulkanCommandAllocator()
	{
		vkDestroyCommandPool(m_pDevice->GetHandle(), m_CommandPoolHandle, nullptr);
	}

	VkCommandBuffer VulkanCommandAllocator::AllocateCommandBuffer()
	{

		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.pNext = nullptr;
		allocateInfo.commandPool = m_CommandPoolHandle;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = 1;

		VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;

		VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pDevice->GetHandle(), &allocateInfo, &cmdBuffer), "Failed to allocate command buffer");

		return cmdBuffer;
	}

	void VulkanCommandAllocator::FreeCommandBuffer(uint32_t count, VkCommandBuffer* cmd)
	{
		vkFreeCommandBuffers(m_pDevice->GetHandle(), m_CommandPoolHandle, 1, cmd);
	}

	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandAllocator* pParantAllocator)
		: m_pAllocator(pParantAllocator)
	{
		m_CommandBufferHandle = m_pAllocator->AllocateCommandBuffer();
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		m_pAllocator->FreeCommandBuffer(1, &m_CommandBufferHandle);
	}

	void VulkanCommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = nullptr;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;
		vkBeginCommandBuffer(m_CommandBufferHandle, &beginInfo);
	}

	void VulkanCommandBuffer::End()
	{
		vkEndCommandBuffer(m_CommandBufferHandle);
	}

	void VulkanCommandBuffer::BeginFrame(const RHIFrameBuffer* pTargetFrameBuffer, const RHIColor clearColor)
	{

		const VulkanFrameBuffer* pTargetFrameBuffer_v = reinterpret_cast<const VulkanFrameBuffer*>(pTargetFrameBuffer);
		
		AddWaitSemaphore(pTargetFrameBuffer_v->GetImageAvailableSemaphore());

		VkRect2D renderArea = {};
		renderArea.offset	= { 0, 0 };
		renderArea.extent	= { pTargetFrameBuffer_v->GetWidth(), pTargetFrameBuffer_v->GetHeight() };

		VkClearValue clearValue			= {};
		clearValue.color.float32[0]		= 0.4f;
		clearValue.color.float32[1]		= 0.3f;
		clearValue.color.float32[2]		= 0.5f;
		clearValue.color.float32[3]		= 1.0f;
		clearValue.depthStencil.depth	= 0.0f;
		clearValue.depthStencil.stencil = 0;

		VkRenderPassBeginInfo beginInfo = {};
		beginInfo.sType					= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.pNext					= nullptr;
		beginInfo.renderPass			= pTargetFrameBuffer_v->GetRenderPass()->GetHandle();
		beginInfo.framebuffer			= pTargetFrameBuffer_v->GetHandle();
		beginInfo.renderArea			= renderArea;
		beginInfo.clearValueCount		= 1;
		beginInfo.pClearValues			= &clearValue;
		
		vkCmdBeginRenderPass(m_CommandBufferHandle, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanCommandBuffer::EndFrame()
	{
		vkCmdEndRenderPass(m_CommandBufferHandle);
	}

	void VulkanCommandBuffer::BindGraphicsPipelineState(const RHIGraphicsPipelineState* pPipelineState)
	{
		const VulkanGraphicsPipelineState* pPipeline = reinterpret_cast<const VulkanGraphicsPipelineState*>(pPipelineState);
		vkCmdBindPipeline(m_CommandBufferHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->GetHandle());
	}

	void VulkanCommandBuffer::BindDescriptorSets(const RHIPipelineLayout* pLayout, const RHIDescriptorSet* pSet)
	{
		const VulkanPipelineLayout* pVulkanLayout = reinterpret_cast<const VulkanPipelineLayout*>(pLayout);
		const VulkanDescriptorSet* pVulkanSet = reinterpret_cast<const VulkanDescriptorSet*>(pSet);
		const VkDescriptorSet handle = pVulkanSet->GetHandle();
		vkCmdBindDescriptorSets(m_CommandBufferHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, pVulkanLayout->GetHandle(), 0, 1, &handle, 0, nullptr);
	}

	void VulkanCommandBuffer::Draw(const RHIVertexBuffer* pVertexBuffer)
	{
		VkDeviceSize offset = 0;

		const VulkanVertexBuffer* pVbo = reinterpret_cast<const VulkanVertexBuffer*>(pVertexBuffer);
		VkBuffer bufferHandle = AsVulkanBuffer(pVbo->GetBuffer())->GetHandle();
		uint32_t vertciesCount = pVbo->GetVertexCount();

		vkCmdBindVertexBuffers(m_CommandBufferHandle, 0, 1, &bufferHandle, &offset);

		vkCmdDraw(m_CommandBufferHandle, vertciesCount, 1, 0, 0);
	}

	void VulkanCommandBuffer::Draw(const RHIVertexBuffer* pVertexBuffer, const RHIIndexBuffer* pIndexBuffer)
	{
		VkDeviceSize offset = 0;

		const VulkanVertexBuffer* pVbo = reinterpret_cast<const VulkanVertexBuffer*>(pVertexBuffer);
		VkBuffer bufferHandle = AsVulkanBuffer(pVbo->GetBuffer())->GetHandle();
		vkCmdBindVertexBuffers(m_CommandBufferHandle, 0, 1, &bufferHandle, &offset);

		const VulkanIndexBuffer* pIbo = reinterpret_cast<const VulkanIndexBuffer*>(pIndexBuffer);
		VkBuffer indexBuffer = AsVulkanBuffer(pIbo->GetBuffer())->GetHandle();
		vkCmdBindIndexBuffer(m_CommandBufferHandle, indexBuffer, offset, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(m_CommandBufferHandle, pIbo->GetIndciesCount(), 1, 0, 0, 0);
	}

	void VulkanCommandBuffer::CopyResource(const RHIBufferCopyDesc& copyDesc)
	{
		VkBufferCopy copyInfo = {};

		copyInfo.srcOffset = copyDesc.SrcOffset;
		copyInfo.dstOffset = copyDesc.DstOffset;
		copyInfo.size = copyDesc.Size;

		VkBuffer srcBuffer = reinterpret_cast<VulkanBuffer*>(copyDesc.SrcBuffer)->GetHandle();
		VkBuffer dstBuffer = reinterpret_cast<VulkanBuffer*>(copyDesc.DstBuffer)->GetHandle();

		vkCmdCopyBuffer(m_CommandBufferHandle, srcBuffer, dstBuffer, 1, &copyInfo);
	}



}
