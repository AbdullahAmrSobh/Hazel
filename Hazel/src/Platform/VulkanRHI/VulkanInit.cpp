#include "hzpch.h"

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanMemory.h"
#include "Platform/VulkanRHI/VulkanQueue.h"
#include "Platform/VulkanRHI/VulkanSwapChain.h"

#include "Platform/VulkanRHI/VulkanShader.h"

#include "Platform/VulkanRHI/VulkanDescriptors.h"

#include "Platform/VulkanRHI/VulkanRHI.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


namespace Hazel 
{
	VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data)
	{
		if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			HZ_CORE_INFO("[{0} : {1}] : {2}", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);
		else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
			HZ_CORE_WARN("[{0} : {1}] : {2}", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);
		else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			HZ_CORE_WARN("[{0} : {1}] : {2}", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);
		else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			HZ_CORE_ERROR("[{0} : {1}] : {2}", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);

		return VK_FALSE;
	}

	VulkanRHI::VulkanRHI(Window* pWindow)
		: RHI(pWindow, RHIBackend::eVulkan)
	{

		InitInstance();

		InitDebugMessenger();

		InitSurface();

		auto physicalDeviceProperties = SelectPhysicalDevice();

		InitDevice(physicalDeviceProperties);

		InitMemoryAllocator();

		InitSwapChain();

		InitShaderCompiler();

		// should this be InitDescriptorsLayoutMangers
		m_pDescriptorsLayoutManager = new VulkanDescriptorsLayoutManager(m_pDevice);
	
	}

	VulkanRHI::~VulkanRHI()
	{
		delete m_pSwapChain;

		delete m_pDevice;

		vkDestroySurfaceKHR(m_InstanceHandle, m_SurfaceHandle, nullptr);

		DestroyDebugUtilsMessenger();

		vkDestroyInstance(m_InstanceHandle, nullptr);
	}

	void VulkanRHI::InitInstance()
	{
		VkApplicationInfo appinfo = {};
		appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appinfo.pNext = nullptr;
		appinfo.pApplicationName = "Hazel App";
		appinfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		appinfo.pEngineName = "Hazel";
		appinfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
		appinfo.apiVersion = VK_API_VERSION_1_2;


		std::vector<const char*> enabledLayers = {
			"VK_LAYER_KHRONOS_validation",
			"VK_LAYER_LUNARG_api_dump",
		};

		std::vector<const char*> enabledExtensions = {};

		enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		uint32_t count;
		const char** pData = glfwGetRequiredInstanceExtensions(&count);
		for (uint32_t index = 0; index < count; index++) enabledExtensions.push_back(pData[index]);

		VkInstanceCreateInfo ci		= {};
		ci.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		ci.pNext					= nullptr;
		ci.flags					= 0;
		ci.pApplicationInfo			= &appinfo;
		ci.enabledLayerCount		= enabledLayers.size();
		ci.ppEnabledLayerNames		= enabledLayers.data();
		ci.enabledExtensionCount	= enabledExtensions.size();
		ci.ppEnabledExtensionNames	= enabledExtensions.data();

		VkResult result = vkCreateInstance(&ci, nullptr, &m_InstanceHandle);

		VK_CHECK_RESULT(result, "Failed to check result");
	}

	void VulkanRHI::InitDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };

		createInfo.pNext = nullptr;
		createInfo.flags = 0;

		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;

		createInfo.pfnUserCallback = debugUtilsMessengerCallback;
		createInfo.pUserData = nullptr;


		auto pfnVkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_InstanceHandle, "vkCreateDebugUtilsMessengerEXT");

		auto result = pfnVkCreateDebugUtilsMessengerEXT(m_InstanceHandle, &createInfo, nullptr, &m_DebugUtilsMessengerHandle);

		VK_CHECK_RESULT(result, "Failed to create a Debug Utils Messenger")
	}

	void VulkanRHI::InitSurface()
	{
		auto nativeWindow = m_pWindow->GetNativeWindow();
		VK_CHECK_RESULT(glfwCreateWindowSurface(m_InstanceHandle, reinterpret_cast<GLFWwindow*>(nativeWindow), nullptr, &m_SurfaceHandle),
			"Failed to create vulkan surface");
	}

	void VulkanRHI::InitDevice(const Ref<VulkanPhysicalDeviceProperties>& pPhysicalDevice)
	{
		m_pDevice = new VulkanDevice(pPhysicalDevice, m_SurfaceHandle);
	}

	void VulkanRHI::InitMemoryAllocator()
	{
		m_pAllocator = new VulkanMemoryAllocator(m_InstanceHandle, m_pDevice);
	}

	void VulkanRHI::InitSwapChain()
	{

		const VulkanPhysicalDeviceProperties& physicalDeviceProperties = m_pDevice->GetPhysicalDeviceProperties();

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.surface = m_SurfaceHandle;
		createInfo.minImageCount = 2;

		auto surfaceFormat = SelectSurfaceFormat(physicalDeviceProperties);
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = SelectSurfaceExtent(physicalDeviceProperties, { m_pWindow->GetWidth(), m_pWindow->GetHeight() });
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (m_pDevice->GetGraphicsQueue().GetFamilyIndex() == m_pDevice->GetPresentQueue().GetFamilyIndex())
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}
		else
		{
			uint32_t queueFamiliesIndcies = {};
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = &queueFamiliesIndcies;
		}

		createInfo.preTransform = physicalDeviceProperties.SurfaceCapabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = SelectSurfacePresentMode(physicalDeviceProperties);
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		m_pSwapChain = new VulkanSwapChain(m_pDevice, &m_pDevice->GetPresentQueue(), createInfo);
	}

	void VulkanRHI::InitShaderCompiler()
	{
		m_pShaderCompiler = new VulkanShaderCompiler(m_pDevice);
	}

	void VulkanRHI::DestroyDebugUtilsMessenger()
	{
		auto pfnVkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_InstanceHandle, "vkDestroyDebugUtilsMessengerEXT");
		pfnVkDestroyDebugUtilsMessengerEXT(m_InstanceHandle, m_DebugUtilsMessengerHandle, nullptr);
	}




	std::vector<VkPhysicalDevice> VulkanRHI::EnmeratePhysicalDevices() const
	{
		std::vector<VkPhysicalDevice> devices;
		uint32_t count;
		vkEnumeratePhysicalDevices(m_InstanceHandle, &count, nullptr);
		devices.resize(count);
		vkEnumeratePhysicalDevices(m_InstanceHandle, &count, devices.data());

		return devices;
	}


	Ref<VulkanPhysicalDeviceProperties> VulkanRHI::GetVulkanPhysicalDeviceProperties(VkPhysicalDevice physicalDeviceHandle, VkSurfaceKHR surfaceHandle) const
	{
		Ref<VulkanPhysicalDeviceProperties> pProperties = CreateRef<VulkanPhysicalDeviceProperties>();

		pProperties->PhysicalDeviceHandle = physicalDeviceHandle;

		uint32_t count = 0;

		// Get all available device's layer
		vkEnumerateDeviceLayerProperties(physicalDeviceHandle, &count, nullptr);
		pProperties->AvailableLayers.resize(count);
		vkEnumerateDeviceLayerProperties(physicalDeviceHandle, &count, pProperties->AvailableLayers.data());

		// Get all available device's extensions
		vkEnumerateDeviceExtensionProperties(physicalDeviceHandle, nullptr, &count, nullptr);
		pProperties->AvailableExtensions.reserve(count);
		vkEnumerateDeviceExtensionProperties(physicalDeviceHandle, nullptr, &count, pProperties->AvailableExtensions.data());

		// Get all available device's features;
		vkGetPhysicalDeviceFeatures(physicalDeviceHandle, &pProperties->Features);

		// Get all available device's queueFamilies;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceHandle, &count, nullptr);
		pProperties->QueueFamilies.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceHandle, &count, pProperties->QueueFamilies.data());

		// Get all available device's memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDeviceHandle, &pProperties->MemoryProperties);

		// Get all available device's surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDeviceHandle, surfaceHandle, &pProperties->SurfaceCapabilities);

		// Get all available device's availableSurfacePresentModes;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceHandle, surfaceHandle, &count, nullptr);
		pProperties->AvailableSurfacePresentModes.reserve(count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceHandle, surfaceHandle, &count, pProperties->AvailableSurfacePresentModes.data());

		// Get all available device's availableSurfaceFormats;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceHandle, surfaceHandle, &count, nullptr);
		pProperties->AvailableSurfaceFormats.resize(count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceHandle, surfaceHandle, &count, pProperties->AvailableSurfaceFormats.data());

		return pProperties;
	}

	inline VkSurfaceFormatKHR VulkanRHI::SelectSurfaceFormat(const VulkanPhysicalDeviceProperties& pProperties) const
	{
		for (const auto& availableFormat : pProperties.AvailableSurfaceFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		return pProperties.AvailableSurfaceFormats[0];
	}

	inline VkPresentModeKHR VulkanRHI::SelectSurfacePresentMode(const VulkanPhysicalDeviceProperties& pProperties) const
	{
		for (const auto& availablePresentMode : pProperties.AvailableSurfacePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	inline VkExtent2D VulkanRHI::SelectSurfaceExtent(const VulkanPhysicalDeviceProperties& pProperties, VkExtent2D actualExtent) const
	{
		if (actualExtent.width != UINT32_MAX)
		{
			return pProperties.SurfaceCapabilities.currentExtent;
		}
		else
		{
			actualExtent.width = std::max(pProperties.SurfaceCapabilities.minImageExtent.width,
				std::min
				(pProperties.SurfaceCapabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(pProperties.SurfaceCapabilities.minImageExtent.height, std::min(pProperties.SurfaceCapabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	inline Ref<VulkanPhysicalDeviceProperties> VulkanRHI::SelectPhysicalDevice() const
	{
		for (VkPhysicalDevice deviceHandle : EnmeratePhysicalDevices())
		{
			return GetVulkanPhysicalDeviceProperties(deviceHandle, m_SurfaceHandle);
		}
	}
}
