#include "hzpch.h"

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/vma/vk_mem_alloc.h"

#include "Platform/VulkanRHI/VulkanMemory.h"


namespace Hazel {

	VulkanMemoryAllocator::VulkanMemoryAllocator(VkInstance instanceHandle, const VulkanDevice* pDevice)
	{
		VmaAllocatorCreateInfo createInfo = {};
		createInfo.flags = 0;
		createInfo.physicalDevice = pDevice->GetPhysicalDeviceHandle();
		createInfo.device = pDevice->GetHandle();
		createInfo.instance = instanceHandle;
		// createInfo.vulkanApiVersion = VK_VERSION_1_2;

		VK_CHECK_RESULT(vmaCreateAllocator(&createInfo, &m_AllocatorHandle),"Failed to create VmaAllocator");
	}

	VulkanMemoryAllocator::~VulkanMemoryAllocator()
	{
		vmaDestroyAllocator(m_AllocatorHandle);
	}

}
