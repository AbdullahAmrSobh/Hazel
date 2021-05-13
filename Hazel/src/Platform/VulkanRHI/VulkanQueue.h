#pragma once
#include <vulkan/vulkan.h>

namespace Hazel
{
	class VulkanDevice;
	class VulkanCommandBuffer;
	class VulkanFence;

	class VulkanQueue
	{
	public:
		VulkanQueue(class VulkanDevice* pDevice, uint32_t familyIndex, uint32_t queueIndex = 0);

		inline VkQueue GetHandle() const { return m_QueueHandle; }
		inline uint32_t GetFamilyIndex() const { return m_FamilyIndex; }
		inline uint32_t GetQueueIndex() const { return m_QueueIndex; }

		void SubmitCommandBuffer(VulkanCommandBuffer* pCommandBuffers, VulkanFence* pFence = nullptr);

	private:
		const class VulkanDevice* m_pDevice;
		VkQueue m_QueueHandle;
		uint32_t m_FamilyIndex, m_QueueIndex;

	};
}
