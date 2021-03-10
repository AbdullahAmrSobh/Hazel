#include "hzpch.h"
#include "Hazel/Core/Window.h"
#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanRHI.h"
#include "Platform/VulkanRHI/VulkanSwapChain.h"

#include "Platform/VulkanRHI/VulkanShader.h"
#include "Platform/VulkanRHI/VulkanPipelineLayout.h"
#include "Platform/VulkanRHI/VulkanGraphicsPipelineState.h"


namespace Hazel {

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		HZ_CORE_ERROR(pCallbackData->pMessage);

		std::string message;
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
			message = "Vulkan [General]";

		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
			message = "Vulkan [General]";

		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
			message = "Vulkan [General]";

		message += std::string(pCallbackData->pMessage);
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			HZ_CORE_WARN(message.c_str());
			
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			HZ_CORE_ERROR(message.c_str());

		return VK_TRUE;
	}


	VulkanRHI::VulkanRHI(Window* pWindow)
		: RHI(pWindow)
	{
			VkApplicationInfo appInfo	= {};
			appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pNext				= nullptr;
			appInfo.pApplicationName	= "No App";
			appInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName			= "Hazel Engine";
			appInfo.engineVersion		= VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion			= VK_VERSION_1_2;
				
			std::vector<const char*> layerNames = EnabledLayers();
			std::vector<const char*> extensionNames = EnabledExtensions();
			
			VkInstanceCreateInfo InstanceCreateInfo		= {};
			InstanceCreateInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceCreateInfo.pNext					= nullptr;
			InstanceCreateInfo.flags					= 0;
			InstanceCreateInfo.pApplicationInfo			= &appInfo;
			InstanceCreateInfo.enabledLayerCount		= layerNames.size();
			InstanceCreateInfo.ppEnabledLayerNames		= layerNames.data();
			InstanceCreateInfo.enabledExtensionCount	= extensionNames.size();
			InstanceCreateInfo.ppEnabledExtensionNames	= extensionNames.data();

			VK_ASSERT(vkCreateInstance(&InstanceCreateInfo, nullptr, &m_InstanceHandle), "Failed to create vulkan instance");

			VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengercreateInfo = {};
			debugUtilsMessengercreateInfo.sType							  = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugUtilsMessengercreateInfo.pNext							  = nullptr;
			debugUtilsMessengercreateInfo.flags				 			  = 0;
			debugUtilsMessengercreateInfo.messageSeverity				  = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
																		  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

			debugUtilsMessengercreateInfo.messageType					  = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
																		  | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
																		  | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

			debugUtilsMessengercreateInfo.pfnUserCallback				  = debugCallback;
			debugUtilsMessengercreateInfo.pUserData						  = nullptr;

			auto pfnVkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_InstanceHandle, "vkCreateDebugUtilsMessengerEXT");
			VK_ASSERT(pfnVkCreateDebugUtilsMessengerEXT(m_InstanceHandle, &debugUtilsMessengercreateInfo, nullptr, &m_DebugMessengerHandle), "Failed to create debug utils messenger");
	

			auto nativeWindow = pWindow->GetNativeWindow();

			VK_ASSERT(
				glfwCreateWindowSurface(m_InstanceHandle, reinterpret_cast<GLFWwindow*>(nativeWindow), nullptr, &m_SurfaceHandle),
				"Failed to create vulkan surface"
			);

			uint32_t count;
			std::vector<VkPhysicalDevice> availableDevices;
			vkEnumeratePhysicalDevices(m_InstanceHandle, &count, nullptr);
			availableDevices.resize(count);
			vkEnumeratePhysicalDevices(m_InstanceHandle, &count, availableDevices.data());

			auto pPhysicalDevice = new VulkanPhysicalDeviceProperties(m_InstanceHandle, availableDevices.front(), m_SurfaceHandle);
			m_Device.Init(pPhysicalDevice);

			VulkanSwapChainDesc swapChainDesc	= {};
			swapChainDesc.surface				= m_SurfaceHandle;
			swapChainDesc.surfaceFormat			= m_Device.GetProperties().SelectSurfaceFormat();
			swapChainDesc.presentMode			= m_Device.GetProperties().SelectSurfacePresentMode();
			swapChainDesc.swapcainExtent		= m_Device.GetProperties().SelectSurfaceExtent({ pWindow->GetWidth(), pWindow->GetHeight() });
			swapChainDesc.transform				= m_Device.GetProperties().SurfaceCapabilities.currentTransform;
			swapChainDesc.minImageCount			= m_Device.GetProperties().SurfaceCapabilities.minImageCount;

			m_pSwapChain = new VulkanSwapChain(&m_Device, swapChainDesc);

			m_pCommandsAllocator = new VulkanCommandsAllocator(&m_Device, 0);
	}

	VulkanRHI::~VulkanRHI()
	{
	
		delete m_pCommandsAllocator;

		vkDestroySurfaceKHR(m_InstanceHandle, m_SurfaceHandle, nullptr);
		auto pfnVkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_InstanceHandle, "vkDestroyDebugUtilsMessengerEXT");
		pfnVkDestroyDebugUtilsMessengerEXT(m_InstanceHandle, m_DebugMessengerHandle, nullptr);
		vkDestroyInstance(m_InstanceHandle, nullptr);
		delete m_pSwapChain;
	}

	Ref<RHIShader> VulkanRHI::CreateShader(const RHIPrecompiledShader& shader) const
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.pNext	= nullptr;
		createInfo.codeSize = shader.GetSize();
		createInfo.pCode = (uint32_t*)shader.GetData();

		VkShaderModule moduleHandle = VK_NULL_HANDLE;
		VK_ASSERT(vkCreateShaderModule(m_Device.GetHandle(), &createInfo, nullptr, &moduleHandle), "Failed to create VkShaderModule");
		
		return CreateScope<VulkanShader>(&m_Device, moduleHandle);
	}

	Scope<RHIPipelineLayout> VulkanRHI::CreatePipelineLayout(const RHIPipelineLayoutDesc& layoutDesc)
	{
		VulkanPipelineLayoutDesc desc(layoutDesc);
		return CreateScope<VulkanPipelineLayout>(&m_Device, desc);
	}

	Scope<RHIGraphicsPipelineState> VulkanRHI::CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& psoDesc)
	{
		VulkanGraphicsPipelineStateDesc desc = VulkanGraphicsPipelineStateDesc(
			psoDesc, 
			reinterpret_cast<VulkanPipelineLayout*>(psoDesc.pPipelineLayout), 
			m_pSwapChain->GetRenderPass()
		);

		return CreateScope<VulkanGraphicsPipelineState>(&m_Device, desc);
	}

	std::vector<Scope<RHICommandBuffer>> VulkanRHI::AllocateCommandBuffer(uint32_t count)
	{
		auto cmdBuffers = m_pCommandsAllocator->AllocateCommandBuffers(count, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		std::vector<Scope<RHICommandBuffer>> ret;
		for (VulkanCommandBuffer* pCmdBuffer : cmdBuffers)
			ret.emplace_back(pCmdBuffer);
		return ret;
	}

	void VulkanRHI::SubmitCommandBuffer(RHICommandBuffer* pCommandBuffer)
	{
	}

	RHISwapChain* VulkanRHI::GetSwapChain()
	{
		return m_pSwapChain;
	}


}
