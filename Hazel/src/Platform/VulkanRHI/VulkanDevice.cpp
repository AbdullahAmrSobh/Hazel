#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanWrapper.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanQueue.h"

#include "Platform/VulkanRHI/VulkanDevice.h"

namespace Hazel 
{

	VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle)
		: m_DeviceHandle(VK_NULL_HANDLE)
		, m_hPhysicalDevice(physicalDevice)
		, m_pPresentQueue(nullptr)
		, m_pGraphicsQueue(nullptr)
		, m_pComputeQueue(nullptr)
		, m_pTransferQueue(nullptr)
	{
		std::vector<const char*> enabledLayers = GetEnabledLayers();
		std::vector<const char*> enabledExtensions = GetEnabledExtensions();
		VkPhysicalDeviceFeatures enabledFeatures = GetEnabledFeatures();
		
		std::vector<VkDeviceQueueCreateInfo> queuesCreateInfo;
		
		uint32_t familyIndex = 0;
		
		float priorities = 1.0f;

		auto deviceQueueFamilyProperties = Vulkan::GetPhysicalDeviceQueueFamilies(physicalDevice);

		for (const auto& qfp : deviceQueueFamilyProperties)
		{
			VkBool32 presentSupport = VK_FALSE;

			VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(
				physicalDevice, familyIndex, surfaceHandle, &presentSupport);
			VK_CHECK_RESULT(result, "Query Failed");

			if (presentSupport && (qfp.queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				VkDeviceQueueCreateInfo createInfo	= {};
				createInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				createInfo.pNext					= nullptr;
				createInfo.flags					= 0;
				createInfo.queueFamilyIndex			= familyIndex;
				createInfo.queueCount				= 1;
				createInfo.pQueuePriorities			= &priorities;

				queuesCreateInfo.push_back(createInfo);
				break;
			}

			familyIndex++;
		}

		VkDeviceCreateInfo createInfo		= {};
		createInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext					= nullptr;
		createInfo.flags					= 0;
		createInfo.queueCreateInfoCount		= queuesCreateInfo.size();
		createInfo.pQueueCreateInfos		= queuesCreateInfo.data();
		createInfo.enabledLayerCount		= enabledLayers.size();
		createInfo.ppEnabledLayerNames		= enabledLayers.data();
		createInfo.enabledExtensionCount	= enabledExtensions.size();
		createInfo.ppEnabledExtensionNames	= enabledExtensions.data();
		createInfo.pEnabledFeatures			= &enabledFeatures;

		VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_DeviceHandle), "Failed to check result");

		 m_pPresentQueue = new VulkanQueue(this, familyIndex);
		m_pGraphicsQueue = m_pPresentQueue;
		m_pComputeQueue = m_pPresentQueue;
		m_pTransferQueue = m_pPresentQueue;
	}

	VulkanDevice::~VulkanDevice()
	{
		vkDeviceWaitIdle(m_DeviceHandle);
		delete m_pPresentQueue;
		vkDestroyDevice(m_DeviceHandle, nullptr);
	}

}

