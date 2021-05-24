#pragma once
#include "Hazel/RHI/RHIObject.h"

#include <vulkan/vulkan.h>

#include <stb_image.h>

namespace Hazel {
	
	class VulkanQueue;

	struct VulkanPhysicalDeviceProperties
	{
		VkPhysicalDevice						PhysicalDeviceHandle;
		std::vector<VkLayerProperties>			AvailableLayers;
		std::vector<VkExtensionProperties>		AvailableExtensions;
		VkPhysicalDeviceFeatures				Features;
		std::vector<VkQueueFamilyProperties>	QueueFamilies;
		VkPhysicalDeviceMemoryProperties		MemoryProperties;
		VkPhysicalDeviceLimits					DeviceLimits;
		VkSurfaceCapabilitiesKHR				SurfaceCapabilities;
		std::vector<VkPresentModeKHR>			AvailableSurfacePresentModes;
		std::vector<VkSurfaceFormatKHR>			AvailableSurfaceFormats;
	};

	class VulkanDevice : private RHIObject
	{
	public:
		VulkanDevice(const Ref<VulkanPhysicalDeviceProperties>& pDeviceProperties, VkSurfaceKHR surfaceHandle);
		~VulkanDevice();

		inline const VulkanPhysicalDeviceProperties& GetPhysicalDeviceProperties() const { return * m_pDeviceProperties.get(); }
		inline VkDevice GetHandle() const { return m_DeviceHandle; }

		inline VulkanQueue& GetPresentQueue() { return *m_pPresentQueue; }
		inline VulkanQueue& GetGraphicsQueue() { return *m_pGraphicsQueue; }
		inline VulkanQueue& GetComputeQueue() { return *m_pComputeQueue; }
		inline VulkanQueue& GetTransferQueue() { return *m_pTransferQueue; }

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
		
		Ref<VulkanPhysicalDeviceProperties> m_pDeviceProperties;

		VulkanQueue* m_pPresentQueue;
		VulkanQueue* m_pGraphicsQueue;
		VulkanQueue* m_pComputeQueue;
		VulkanQueue* m_pTransferQueue;

	};
}
