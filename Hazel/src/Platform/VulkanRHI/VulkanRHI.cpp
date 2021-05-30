#include "hzpch.h"

#include <vulkan/vulkan.h>

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanQueue.h"
#include "Platform/VulkanRHI/VulkanMemory.h"
#include "Platform/VulkanRHI/VulkanSwapChain.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"
#include "Platform/VulkanRHI/VulkanShader.h"
#include "Platform/VulkanRHI/VulkanPipelineState.h"
#include "Platform/VulkanRHI/VulkanCommandBuffers.h"
#include "Platform/VulkanRHI/VulkanDescriptors.h"
#include "Platform/VulkanRHI/VulkanResources.h"
#include "Platform/VulkanRHI/VulkanRHI.h"

#include "Platform/VulkanRHI/VulkanWrapper.h"

namespace Hazel
{
	VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type,
		const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
		void *user_data)
	{
		if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			HZ_CORE_INFO("[{0} : {1}] : {2}", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);
		else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
			HZ_CORE_WARN("[{0} : {1}] : {2}", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);
		else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			HZ_CORE_WARN("{0} \n : \n {1} : \n {2}", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);
		else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			std::stringstream msg;
			msg << callback_data->pMessageIdName << " : " << callback_data->pMessage;
			HZ_CORE_ASSERT(false, msg.str().c_str());
		}

		return VK_FALSE;
	}

	VulkanRHI::VulkanRHI(Window *pWindow)
		: RHI(pWindow, RHIBackend::eVulkan)
	{
		{
			VkApplicationInfo appInfo = {};
			{
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pNext = nullptr;
				appInfo.pApplicationName = "Hazel App";
				appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
				appInfo.pEngineName = "Hazel";
				appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
				appInfo.apiVersion = VK_API_VERSION_1_2;
			}

			std::vector<const char *> enabledLayers = {
				"VK_LAYER_KHRONOS_validation",
				// "VK_LAYER_LUNARG_api_dump",
			};

			std::vector<const char *> enabledExtensions = {};

			enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

			uint32_t count;
			const char **pData = glfwGetRequiredInstanceExtensions(&count);
			for (uint32_t index = 0; index < count; index++)
				enabledExtensions.push_back(pData[index]);

			VkInstanceCreateInfo instanceInfo = {};
			{
				instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				instanceInfo.pNext = nullptr;
				instanceInfo.flags = 0;
				instanceInfo.pApplicationInfo = &appInfo;
				instanceInfo.enabledLayerCount = enabledLayers.size();
				instanceInfo.ppEnabledLayerNames = enabledLayers.data();
				instanceInfo.enabledExtensionCount = enabledExtensions.size();
				instanceInfo.ppEnabledExtensionNames = enabledExtensions.data();
			}

			VK_CHECK_RESULT(vkCreateInstance(&instanceInfo, nullptr, &m_InstanceHandle), "Failed to check result");
		}

		{
			VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
			{

				debugUtilsMessengerInfo.pNext = nullptr;
				debugUtilsMessengerInfo.flags = 0;
				debugUtilsMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
				debugUtilsMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
				debugUtilsMessengerInfo.pfnUserCallback = debugUtilsMessengerCallback;
				debugUtilsMessengerInfo.pUserData = nullptr;
			}

			auto pfnVkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_InstanceHandle, "vkCreateDebugUtilsMessengerEXT");

			VK_CHECK_RESULT(pfnVkCreateDebugUtilsMessengerEXT(m_InstanceHandle, &debugUtilsMessengerInfo, nullptr, &m_DebugUtilsMessengerHandle), "Failed to create a Debug Utils Messenger");
		}

		{
			m_SurfaceHandle = Vulkan::GetSurface(m_InstanceHandle, reinterpret_cast<GLFWwindow *>(m_pWindow->GetNativeWindow()));

			std::vector<VkPhysicalDevice> physicalDevices = Vulkan::EnmeratePhysicalDevices(m_InstanceHandle);
			auto physicalDeviceProperties = SelectPhysicalDevice(physicalDevices);

			m_pDevice = new VulkanDevice(physicalDeviceProperties, m_SurfaceHandle);
		}

		m_pAllocator = new VulkanMemoryAllocator(m_InstanceHandle, m_pDevice);
		m_pSwapChain = new VulkanSwapChain(m_pDevice, m_SurfaceHandle, &m_pDevice->GetPresentQueue());
		m_pShaderCompiler = new VulkanShaderCompiler(m_pDevice);
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

	void VulkanRHI::DestroyDebugUtilsMessenger()
	{
		auto pfnVkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_InstanceHandle, "vkDestroyDebugUtilsMessengerEXT");
		pfnVkDestroyDebugUtilsMessengerEXT(m_InstanceHandle, m_DebugUtilsMessengerHandle, nullptr);
	}

	VkPhysicalDevice VulkanRHI::SelectPhysicalDevice(const std::vector<VkPhysicalDevice> &physicalDevices) const
	{
		return physicalDevices.front();
	}

	void VulkanRHI::OnInit()
	{
		m_pCommandAllocator = new VulkanCommandAllocator(m_pDevice, &m_pDevice->GetGraphicsQueue());
	}

	void VulkanRHI::OnShutdown()
	{
		delete m_pCommandAllocator;
	}

	void VulkanRHI::OnUpdate()
	{
	}

	RHISwapChain *VulkanRHI::GetSwapChain()
	{
		return reinterpret_cast<RHISwapChain *>(m_pSwapChain);
	}

	RHIShaderCompiler *VulkanRHI::GetShaderCompiler()
	{
		return reinterpret_cast<RHIShaderCompiler *>(m_pShaderCompiler);
	}

}
