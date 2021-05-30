#include "hzpch.h"

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanQueue.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"
#include "Platform/VulkanRHI/VulkanPipelineState.h"
#include "Platform/VulkanRHI/VulkanResources.h"

#include "Platform/VulkanRHI/VulkanDescriptors.h"

#include "Platform/VulkanRHI/VulkanCommandBuffers.h"
#include "Platform/VulkanRHI/VulkanRHI.h"


namespace Hazel {


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



	VulkanCommandAllocator::VulkanCommandAllocator(const VulkanDevice* pDevice, const VulkanQueue* pQueue)
		: m_pDevice(pDevice)
	{

		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT;
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

	void VulkanCommandBuffer::Reset()
	{
		VkCommandBufferResetFlags resetFlags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
		VK_CHECK_RESULT(vkResetCommandBuffer(m_CommandBufferHandle, resetFlags), "Failed to Rest command buffer");
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
		
		AddWaitSemaphore(pTargetFrameBuffer_v->GetImageAvailableSemaphore().GetHandle());

		VkRect2D renderArea = {};
		renderArea.offset	= { 0, 0 };
		renderArea.extent	= { pTargetFrameBuffer_v->GetWidth(), pTargetFrameBuffer_v->GetHeight() };

		std::vector<VkClearValue> clearValues = { { clearColor.R, clearColor.G, clearColor.B, clearColor.A  } };
		// clearValues[0].depthStencil.depth	= 0.0f;
		// clearValues[0].depthStencil.stencil = 0;

		VkRenderPassBeginInfo beginInfo = {};
		beginInfo.sType					= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.pNext					= nullptr;
		beginInfo.renderPass			= pTargetFrameBuffer_v->GetRenderPass()->GetHandle();
		beginInfo.framebuffer			= pTargetFrameBuffer_v->GetHandle();
		beginInfo.renderArea			= renderArea;
		beginInfo.clearValueCount		= clearValues.size();
		beginInfo.pClearValues			= clearValues.data();
		
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
		VkBuffer bufferHandle = reinterpret_cast<VulkanBuffer*>(pVbo->GetBuffer())->GetHandle();
		uint32_t vertciesCount = pVbo->GetVertexCount();

		vkCmdBindVertexBuffers(m_CommandBufferHandle, 0, 1, &bufferHandle, &offset);

		vkCmdDraw(m_CommandBufferHandle, vertciesCount, 1, 0, 0);
	}

	void VulkanCommandBuffer::Draw(const RHIVertexBuffer* pVertexBuffer, const RHIIndexBuffer* pIndexBuffer)
	{
		VkDeviceSize offset = 0;

		const VulkanVertexBuffer* pVbo = reinterpret_cast<const VulkanVertexBuffer*>(pVertexBuffer);
		VkBuffer bufferHandle = reinterpret_cast<VulkanBuffer*>(pVbo->GetBuffer())->GetHandle();
		vkCmdBindVertexBuffers(m_CommandBufferHandle, 0, 1, &bufferHandle, &offset);

		const VulkanIndexBuffer* pIbo = reinterpret_cast<const VulkanIndexBuffer*>(pIndexBuffer);
		VkBuffer indexBuffer = reinterpret_cast<VulkanBuffer*>(pIbo->GetBuffer())->GetHandle();
		vkCmdBindIndexBuffer(m_CommandBufferHandle, indexBuffer, offset, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(m_CommandBufferHandle, pIbo->GetIndciesCount(), 1, 0, 0, 0);
	}

	void VulkanCommandBuffer::CopyResource(const RHIBufferCopyDesc& copyDesc)
	{
		VkBufferCopy copyInfo = {};

		copyInfo.srcOffset = copyDesc.SrcOffset;
		copyInfo.dstOffset = copyDesc.DstOffset;
		copyInfo.size		= copyDesc.Size;

		VkBuffer srcBuffer = reinterpret_cast<VulkanBuffer*>(copyDesc.SrcBuffer)->GetHandle();
		VkBuffer dstBuffer = reinterpret_cast<VulkanBuffer*>(copyDesc.DstBuffer)->GetHandle();

		vkCmdCopyBuffer(m_CommandBufferHandle, srcBuffer, dstBuffer, 1, &copyInfo);
	}

	void VulkanCommandBuffer::CopyResource(const RHITextureCopyDesc& copyDesc)
	{
		HZ_CORE_ASSERT(false, "This method is not implemented yet");
	}

	void VulkanCommandBuffer::CopyResource(const RHITextureToBufferCopyDesc& copyDesc)
	{
		HZ_CORE_ASSERT(false, "This method is not implemented yet");
	}

	void VulkanCommandBuffer::CopyResource(const RHIBufferToTextureCopyDesc& copyDesc)
	{
		VkBuffer srcBuffer = reinterpret_cast<const VulkanBuffer*>(copyDesc.pSrcBuffer)->GetHandle();
		VkImage	 dstImage = reinterpret_cast<VulkanImage*>(copyDesc.pDstTexture)->GetHandle();

		VkImageSubresourceRange subresourceRange = {};
		// Image only contains color data
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		// Start at first mip level
		subresourceRange.baseArrayLayer = 0;
		// We will transition on all mip levels
		subresourceRange.levelCount = 1;
		// The 2D texture only has one layer
		subresourceRange.layerCount = 1;
		subresourceRange.baseMipLevel = 0;

		VkImageMemoryBarrier barrier{};
		barrier.sType								= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.pNext								= nullptr;
		barrier.srcAccessMask						= 0;
		barrier.dstAccessMask						= VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.oldLayout							= VK_IMAGE_LAYOUT_UNDEFINED;
		barrier.newLayout							= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.srcQueueFamilyIndex					= VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex					= VK_QUEUE_FAMILY_IGNORED;
		barrier.image								= dstImage;
		barrier.subresourceRange					= subresourceRange;

		vkCmdPipelineBarrier(m_CommandBufferHandle, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		VkBufferImageCopy imageCopy					= {};
		imageCopy.bufferOffset						= copyDesc.SrcOffset;
		imageCopy.bufferRowLength					= 0;
		imageCopy.bufferImageHeight					= 0;
		imageCopy.imageSubresource.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopy.imageSubresource.baseArrayLayer	= 0;
		imageCopy.imageSubresource.layerCount		= 1;
		imageCopy.imageSubresource.mipLevel			= 0;
		imageCopy.imageOffset						= { 0, 0, 0 };
		imageCopy.imageExtent						= { copyDesc.Width, copyDesc.Height, 1 };

		vkCmdCopyBufferToImage(m_CommandBufferHandle, srcBuffer, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);

		barrier.srcAccessMask					= VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask					= VK_ACCESS_SHADER_READ_BIT;
		barrier.oldLayout						= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout						= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		vkCmdPipelineBarrier(m_CommandBufferHandle, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}

}
