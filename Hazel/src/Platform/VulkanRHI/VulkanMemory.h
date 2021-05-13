#pragma once
#include "Platform/VulkanRHI/vma/vk_mem_alloc.h"

#include <vulkan/vulkan.h>


namespace Hazel {

	class VulkanMemoryAllocator
	{
	public:
		VulkanMemoryAllocator(VkInstance instanceHandle, const class VulkanDevice* pDevice);
		~VulkanMemoryAllocator();

		inline VmaAllocator GetHandle() const { return m_AllocatorHandle; }

		inline VkResult AllocateBuffer(const VkBufferCreateInfo& bufferCreateInfo, const VmaAllocationCreateInfo& allocationInfo, VkBuffer* pOutBuffer, VmaAllocation* pOutAllocation) const
		{
			return vmaCreateBuffer(m_AllocatorHandle, &bufferCreateInfo, &allocationInfo, pOutBuffer, pOutAllocation, nullptr);
		}

	private:
		const VulkanDevice* m_pDevice;
		VmaAllocator m_AllocatorHandle;
	};
}
