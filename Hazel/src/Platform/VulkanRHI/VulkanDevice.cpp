#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanDevice.h"

namespace Hazel {
	
	VulkanDevice::~VulkanDevice()
	{
		delete m_pPhysicalDeviceProperties;
		vkDestroyDevice(m_DeviceHandle, nullptr);
		vmaDestroyAllocator(m_AllocatorHandle);
	}

	void VulkanDevice::Init(VulkanPhysicalDeviceProperties* pPhysicalDeviceProperties)
	{
		m_pPhysicalDeviceProperties = pPhysicalDeviceProperties;

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		uint32_t gfxQueueFamilyIndex
			, presentQueueFamilyIndex;

		for (uint32_t familyIndex = 0; familyIndex < m_pPhysicalDeviceProperties->QueueFamilies.size(); familyIndex++)
		{
			const VkQueueFamilyProperties currentFamily = m_pPhysicalDeviceProperties->QueueFamilies[familyIndex];
			VkBool32 presentSupport = VK_FALSE;
			VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(m_pPhysicalDeviceProperties->PhysicalDeviceHandle, familyIndex, m_pPhysicalDeviceProperties->SurfaceHandle, &presentSupport);
			VK_ASSERT(result, "Failed to check present support on this queue and surface");

			if ((currentFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT && presentSupport == VK_TRUE)
			{
				float priority = 1.0f;

				VkDeviceQueueCreateInfo currentCreateInfo = {};
				currentCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				currentCreateInfo.pNext = NULL;
				currentCreateInfo.flags = 0;
				currentCreateInfo.queueFamilyIndex = familyIndex;
				currentCreateInfo.queueCount = 1;
				currentCreateInfo.pQueuePriorities = &priority;

				queueCreateInfos.push_back(currentCreateInfo);
				gfxQueueFamilyIndex = familyIndex;
				presentSupport = familyIndex;

				break;
			}
			else
			{
				gfxQueueFamilyIndex = 0;
				presentQueueFamilyIndex = 0;
			}
		}

		VkPhysicalDeviceFeatures features = {};

		VkDeviceCreateInfo deviceCreateInfo		 = {};
		deviceCreateInfo.sType					 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pNext					 = nullptr;
		deviceCreateInfo.flags					 = 0;
		deviceCreateInfo.queueCreateInfoCount	 = queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos		 = queueCreateInfos.data();
		deviceCreateInfo.enabledLayerCount		 = 0;
		deviceCreateInfo.ppEnabledLayerNames	 = nullptr;

		std::vector<const char*> pExtensions = { "VK_KHR_swapchain" };
		deviceCreateInfo.enabledExtensionCount	 = 1;
		deviceCreateInfo.ppEnabledExtensionNames = pExtensions.data();

		deviceCreateInfo.pEnabledFeatures = &features;

		VK_ASSERT(vkCreateDevice(m_pPhysicalDeviceProperties->PhysicalDeviceHandle, &deviceCreateInfo, nullptr, &m_DeviceHandle), "Failed to create a vulkan logical device");

		VkQueue queueHandle = VK_NULL_HANDLE;
		vkGetDeviceQueue(m_DeviceHandle, gfxQueueFamilyIndex, 0, &queueHandle);

		m_PresentQueue.Init(queueHandle);
		m_GraphicsQueue.Init(queueHandle);
		m_ComputeQueue.Init(queueHandle);
		m_TransferQueue.Init(queueHandle);


		VmaAllocatorCreateInfo allocatorCreateInfo	= {};
		allocatorCreateInfo.instance				= m_pPhysicalDeviceProperties->InstanceHandle;
		allocatorCreateInfo.physicalDevice			= m_pPhysicalDeviceProperties->PhysicalDeviceHandle;
		allocatorCreateInfo.device					= m_DeviceHandle;

		VK_ASSERT(vmaCreateAllocator(&allocatorCreateInfo, &m_AllocatorHandle), "Failed to create Vulkan Memory allocator object (vma)");
	}

}
