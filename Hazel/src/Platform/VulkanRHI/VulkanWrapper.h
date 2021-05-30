#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "Hazel/Core/Base.h"
#include "Platform/VulkanRHI/VulkanUtils.h"

namespace Hazel {
	namespace Vulkan {

		static inline VkSurfaceKHR GetSurface(VkInstance instance, GLFWwindow* pWindow)
		{
			VkSurfaceKHR surface = VK_NULL_HANDLE;
			VK_CHECK_RESULT(glfwCreateWindowSurface(instance, pWindow, nullptr, &surface),
				"Failed to create vulkan surface");
			return surface;
		}

		static inline std::vector<VkLayerProperties> GetPhysicalDevicAvailableLayers(VkPhysicalDevice handle)
		{
			std::vector<VkLayerProperties> layers;
			uint32_t count = 0;
			vkEnumerateDeviceLayerProperties(handle, &count, nullptr);
			layers.resize(count);
			vkEnumerateDeviceLayerProperties(handle, &count, layers.data());
			return layers;
		}

		static inline std::vector<VkExtensionProperties> GetPhysicalDevicAvailableExtensions(VkPhysicalDevice physicalDevice, const char* layerName = nullptr)
		{
			std::vector<VkExtensionProperties> extensions;
			uint32_t count = 0;
			vkEnumerateDeviceExtensionProperties(physicalDevice, layerName, &count, nullptr);
			extensions.resize(count);
			vkEnumerateDeviceExtensionProperties(physicalDevice, layerName, &count, extensions.data());
			return extensions;
		}

		static inline VkPhysicalDeviceFeatures GetPhysicalDeviceAvailableFeatures(VkPhysicalDevice physicalDevice)
		{
			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(physicalDevice, &features);
			return features;
		}

		static inline std::vector<VkQueueFamilyProperties> GetPhysicalDeviceQueueFamilies(VkPhysicalDevice physcialDevice)
		{
			std::vector<VkQueueFamilyProperties> queueFamilies;
			uint32_t count;
			vkGetPhysicalDeviceQueueFamilyProperties(physcialDevice, &count, nullptr);
			queueFamilies.resize(count);
			vkGetPhysicalDeviceQueueFamilyProperties(physcialDevice, &count, queueFamilies.data());

			return queueFamilies;
		}

		static inline VkPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice)
		{
			VkPhysicalDeviceMemoryProperties props;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &props);
			return props;
		}

		static inline VkSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice physcialDevice, VkSurfaceKHR surface)
		{
			VkSurfaceCapabilitiesKHR capas;
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physcialDevice, surface, &capas);
			return capas;
		}

		static inline std::vector<VkPresentModeKHR> GetPhysicalDeviceSurfaceAvailablePresentModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
		{
			std::vector<VkPresentModeKHR> presentModes = {};
			uint32_t count;
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, nullptr);
			presentModes.reserve(count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, presentModes.data());

			return presentModes;

		}

		static inline std::vector<VkSurfaceFormatKHR> GetPhysicalDeviceSurfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
		{
			std::vector<VkSurfaceFormatKHR> surfaceFormats;
			uint32_t count;

			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);
			surfaceFormats.resize(count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, surfaceFormats.data());

			return surfaceFormats;
		}

		static inline std::vector<VkPhysicalDevice> EnmeratePhysicalDevices(VkInstance instance) 
		{
			std::vector<VkPhysicalDevice> devices;

			uint32_t count;
			vkEnumeratePhysicalDevices(instance, &count, nullptr);
			devices.resize(count);
			vkEnumeratePhysicalDevices(instance, &count, devices.data());

			return devices;
		}

		static inline VkPhysicalDeviceProperties GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice)
		{
			VkPhysicalDeviceProperties props = {};
			vkGetPhysicalDeviceProperties(physicalDevice, &props);
			return props;
		}

		static inline std::vector<VkImage> GetSwapchainImages(VkDevice deviceHandle, VkSwapchainKHR swapchainHandle)
		{
			std::vector<VkImage> images;
			uint32_t count;
			vkGetSwapchainImagesKHR(deviceHandle, swapchainHandle, &count, nullptr);
			images.resize(count);
			vkGetSwapchainImagesKHR(deviceHandle, swapchainHandle, &count, images.data());

			return images;
		}

	}

}
