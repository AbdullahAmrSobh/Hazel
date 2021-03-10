#pragma once
#include <vulkan/vulkan.h>

namespace Hazel {

	class VulkanCommandBuffer;
	class VulkanQueue
	{
	public:
		void Init(VkQueue queue);

		void Execute(VulkanCommandBuffer* pCommandBuffer, VkSemaphore imageAvailableSemaphore, VkSemaphore signalSemaphores) const;

		inline VkQueue GetHandle() const { return m_QueueHandle; }
	private:
		VkQueue m_QueueHandle;
	};
}
