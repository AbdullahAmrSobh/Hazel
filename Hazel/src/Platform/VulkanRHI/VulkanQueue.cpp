#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"

#include "Platform/VulkanRHI/VulkanCommandBuffers.h"
#include "Platform/VulkanRHI/VulkanResources.h"
#include "Platform/VulkanRHI/VulkanQueue.h"


namespace Hazel {


	VulkanQueue::VulkanQueue(VulkanDevice* pDevice, uint32_t familyIndex, uint32_t queueIndex)
		: m_pDevice(pDevice), m_QueueHandle(VK_NULL_HANDLE), m_FamilyIndex(familyIndex), m_QueueIndex(queueIndex)
	{
		vkGetDeviceQueue(m_pDevice->GetHandle(), m_FamilyIndex, m_QueueIndex, &m_QueueHandle);
	}

	void VulkanQueue::SubmitCommandBuffer(VulkanCommandBuffer* pCommandBuffers, VulkanFence* pFence)
	{
		std::vector<VkSemaphore> waitSemaphore{ pCommandBuffers->GetWaitSemaphores() };
		std::vector<VkPipelineStageFlags> stageFlags{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		std::vector<VkSemaphore> signalSemaphore{ };

		VkCommandBuffer cmdBuffers = pCommandBuffers->GetHandle();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext					= nullptr;
		submitInfo.waitSemaphoreCount		= waitSemaphore.size();
		submitInfo.pWaitSemaphores			= waitSemaphore.data();
		submitInfo.pWaitDstStageMask		= stageFlags.data();
		submitInfo.commandBufferCount		= 1;
		submitInfo.pCommandBuffers			= &cmdBuffers;
		submitInfo.signalSemaphoreCount		= signalSemaphore.size();
		submitInfo.pSignalSemaphores		= signalSemaphore.data();

		vkQueueSubmit(m_QueueHandle, 1, &submitInfo, pFence != nullptr ? pFence->GetHandle() : VK_NULL_HANDLE);
		vkQueueWaitIdle(m_QueueHandle);
	}

}
