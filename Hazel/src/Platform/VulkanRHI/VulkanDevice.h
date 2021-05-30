#pragma once
#include <vulkan/vulkan.h>

namespace Hazel {
	
	class VulkanQueue;

	class VulkanDevice 
	{
	public:
		VulkanDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle);
		~VulkanDevice();

		inline VkDevice GetHandle() const { return m_DeviceHandle; }
		inline VkPhysicalDevice GetPhysicalDeviceHandle() const { return m_hPhysicalDevice; }

		inline VulkanQueue& GetPresentQueue() const { return *m_pPresentQueue; }
		inline VulkanQueue& GetGraphicsQueue() const { return *m_pGraphicsQueue; }
		inline VulkanQueue& GetComputeQueue() const { return *m_pComputeQueue; }
		inline VulkanQueue& GetTransferQueue() const { return *m_pTransferQueue; }

	private:

		inline VkPhysicalDeviceFeatures GetEnabledFeatures() const
		{
			VkPhysicalDeviceFeatures features = {};

			return features;
		}

		inline std::vector<const char*> GetEnabledLayers() const
		{
			std::vector<const char*> enabledLayers = {};

			return enabledLayers;
		}
		
		inline std::vector<const char*> GetEnabledExtensions() const
		{
			std::vector<const char*> enabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

			return enabledExtensions;
		}

	private:
		VkDevice m_DeviceHandle;
		
		VkPhysicalDevice m_hPhysicalDevice;

		mutable VulkanQueue* m_pPresentQueue;
		mutable VulkanQueue* m_pGraphicsQueue;
		mutable VulkanQueue* m_pComputeQueue;
		mutable VulkanQueue* m_pTransferQueue;

	};
}
