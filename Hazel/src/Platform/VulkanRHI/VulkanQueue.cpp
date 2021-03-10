#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanCommandBuffer.h"
#include "Platform/VulkanRHI/VulkanQueue.h"

namespace Hazel {

	void VulkanQueue::Init(VkQueue queue)
	{
		m_QueueHandle = queue;
	}

	void VulkanQueue::Execute(VulkanCommandBuffer* pCommandBuffer, VkSemaphore imageAvailableSemaphore, VkSemaphore signalSemaphores) const
	{

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkCommandBuffer commandBuffers[] = { pCommandBuffer->GetHandle() };
		VkSemaphore SignalSemaphores[] = { signalSemaphores };

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = commandBuffers;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = SignalSemaphores;

		VK_ASSERT(vkQueueSubmit(m_QueueHandle, 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit")
	}

}
