#pragma once
#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanQueue.h"

namespace Hazel
{
	struct VulkanPhysicalDeviceProperties
	{
		VulkanPhysicalDeviceProperties(VkInstance instanceHandle, VkPhysicalDevice physicalDeviceHandle, VkSurfaceKHR surfaceHandle)
			: InstanceHandle(instanceHandle), PhysicalDeviceHandle(physicalDeviceHandle), SurfaceHandle(surfaceHandle)
		{
			uint32_t count;

			std::vector<VkLayerProperties> availableLayers;
			vkEnumerateDeviceLayerProperties(PhysicalDeviceHandle, &count, nullptr);
			AvailableLayers.resize(count);
			vkEnumerateDeviceLayerProperties(PhysicalDeviceHandle, &count, AvailableLayers.data());

			std::vector<VkExtensionProperties> availableExtensions;
			vkEnumerateDeviceExtensionProperties(PhysicalDeviceHandle, nullptr, &count, nullptr);
			AvailableExtensions.reserve(count);
			vkEnumerateDeviceExtensionProperties(PhysicalDeviceHandle, nullptr, &count, AvailableExtensions.data());


			//VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(PhysicalDeviceHandle, &Features);

			//std::vector<VkQueueFamilyProperties> queueFamilies;
			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDeviceHandle, &count, nullptr);
			QueueFamilies.resize(count);
			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDeviceHandle, &count, QueueFamilies.data());

			//VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(PhysicalDeviceHandle, &MemoryProperties);

			//VkSurfaceCapabilitiesKHR surfaceCapabilities;
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDeviceHandle, SurfaceHandle, &SurfaceCapabilities);

			//std::vector<VkPresentModeKHR>	availableSurfacePresentModes;
			vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDeviceHandle, SurfaceHandle, &count, nullptr);
			AvailableSurfacePresentModes.reserve(count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDeviceHandle, SurfaceHandle, &count, AvailableSurfacePresentModes.data());

			//std::vector<VkSurfaceFormatKHR> availableSurfaceFormats;
			vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDeviceHandle, SurfaceHandle, &count, nullptr);
			AvailableSurfaceFormats.resize(count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDeviceHandle, SurfaceHandle, &count, AvailableSurfaceFormats.data());
		}

		inline VkSurfaceFormatKHR SelectSurfaceFormat() const
		{
			for (const auto& availableFormat : AvailableSurfaceFormats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}
			return AvailableSurfaceFormats[0];
		}

		inline VkPresentModeKHR SelectSurfacePresentMode() const
		{
			for (const auto& availablePresentMode : AvailableSurfacePresentModes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return availablePresentMode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}

		inline VkExtent2D SelectSurfaceExtent(VkExtent2D actualExtent) const
		{
			if (SurfaceCapabilities.currentExtent.width != UINT32_MAX) {
				return SurfaceCapabilities.currentExtent;
			}
			else {
				actualExtent.width = std::max(SurfaceCapabilities.minImageExtent.width, std::min(SurfaceCapabilities.maxImageExtent.width, actualExtent.width));
				actualExtent.height = std::max(SurfaceCapabilities.minImageExtent.height, std::min(SurfaceCapabilities.maxImageExtent.height, actualExtent.height));

				return actualExtent;
			}
		}

		VkInstance								InstanceHandle;
		VkPhysicalDevice						PhysicalDeviceHandle;
		VkSurfaceKHR							SurfaceHandle;

		std::vector<VkLayerProperties>			AvailableLayers;
		std::vector<VkExtensionProperties>		AvailableExtensions;


		VkPhysicalDeviceFeatures				Features;
		std::vector<VkQueueFamilyProperties>	QueueFamilies;
		VkPhysicalDeviceMemoryProperties		MemoryProperties;
		// VkPhysicalDeviceLimits				DeviceLimits;

		VkSurfaceCapabilitiesKHR				SurfaceCapabilities;
		std::vector<VkPresentModeKHR>			AvailableSurfacePresentModes;
		std::vector<VkSurfaceFormatKHR>			AvailableSurfaceFormats;
	};

	class VulkanDevice
	{
	public:
		VulkanDevice() = default;
		VulkanDevice(const VulkanDevice*) = delete;
		~VulkanDevice();

		void Init(VulkanPhysicalDeviceProperties* pPhysicalDeviceProperties);

		inline VkDevice GetHandle() const { return m_DeviceHandle; }
		inline VmaAllocator GetVmaHandle() const { return m_AllocatorHandle; }
		inline const VulkanPhysicalDeviceProperties& GetProperties() const { return *m_pPhysicalDeviceProperties; }

		inline const VulkanQueue& GetPresentQueue() const { return m_PresentQueue; }
		inline const VulkanQueue& GetGraphicsQueue() const { return m_GraphicsQueue; }
		inline const VulkanQueue& GetComputeQueue() const { return m_ComputeQueue; }
		inline const VulkanQueue& GetTransferQueue() const { return m_TransferQueue; }

	private:
		VkDevice								m_DeviceHandle;
		VmaAllocator							m_AllocatorHandle;
		VulkanPhysicalDeviceProperties*			m_pPhysicalDeviceProperties;

		VulkanQueue								m_PresentQueue;
		VulkanQueue								m_GraphicsQueue;
		VulkanQueue								m_ComputeQueue;
		VulkanQueue								m_TransferQueue;
	};

}
