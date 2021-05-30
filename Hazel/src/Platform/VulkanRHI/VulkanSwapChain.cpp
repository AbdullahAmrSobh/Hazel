#include "hzpch.h"

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanQueue.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"
#include "Platform/VulkanRHI/VulkanResources.h"
#include "Platform/VulkanRHI/VulkanSwapChain.h"

#include "Platform/VulkanRHI/VulkanWrapper.h"
#include "Platform/VulkanRHI/VulkanUtils.h"

namespace Hazel {

	VulkanSwapChain::VulkanSwapChain(const VulkanDevice* pDevice, VkSurfaceKHR hSurface, const VulkanQueue* pQueue)
		: m_pDevice(pDevice)
		, m_pPresentQueue(pQueue)
		, m_hSwapchain(VK_NULL_HANDLE)
		, m_hSurface(hSurface)
		, m_pRenderPass(nullptr)
		, m_PresentCompleteSemaphore(new VulkanSemaphore(m_pDevice))
	{
		auto extent = QueryCurrentExtent();
		InitSwapchain(extent.width, extent.height );
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		// Wait for the device to become idle
		VK_CHECK_RESULT(vkDeviceWaitIdle(m_pDevice->GetHandle()));

		for ( uint32_t i = 0; i < m_pFrameBuffers.size(); i++)
			delete m_pFrameBuffers[i];

		vkDestroySwapchainKHR(m_pDevice->GetHandle(), m_hSwapchain, nullptr);
	}

	void VulkanSwapChain::OnResize(uint32_t width, uint32_t height)
	{
		HZ_CORE_INFO("This method is not implemented yet");
	}

	void VulkanSwapChain::Present()
	{
		std::vector<VkSemaphore> framebufferReadySemaphore = { m_PresentCompleteSemaphore->GetHandle() };

		static VkResult presentationResult = VK_RESULT_MAX_ENUM;

		VkPresentInfoKHR presentInfo	= {};
		presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext				= nullptr;
		presentInfo.waitSemaphoreCount	= framebufferReadySemaphore.size();
		presentInfo.pWaitSemaphores		= framebufferReadySemaphore.data();
		presentInfo.swapchainCount		= 1;
		presentInfo.pSwapchains			= &m_hSwapchain;
		presentInfo.pImageIndices		= &m_CurrentFrameIndex;
		presentInfo.pResults			= &presentationResult;

		auto presentSubmitResult= vkQueuePresentKHR(m_pPresentQueue->GetHandle(), &presentInfo);
		// VK_CHECK_RESULT(presentSubmitResult, "Failed to present image[{0}]", m_CurrentFrameIndex);
		// HZ_CORE_ASSERT(presentationResult == VK_SUCCESS, "Failed to present");

		if (presentSubmitResult != VK_SUCCESS)
		{
			HZ_CORE_ERROR("present errors {0}, {1}", VulkanUtils::ResultToString(presentationResult).c_str(), VulkanUtils::ResultToString(presentSubmitResult).c_str());
			HZ_CORE_ERROR("Attempting to recreate swapchain");
			auto extent = QueryCurrentExtent();
			InitSwapchain(extent.width, extent.height);
		}

		vkQueueWaitIdle(m_pPresentQueue->GetHandle());

	}

	RHIFrameBuffer* VulkanSwapChain::AcquireNextFrame()
	{
		VkSemaphore semaphore = m_PresentCompleteSemaphore->GetHandle();
		// VkSemaphore semaphore = m_pFrameBuffers[m_CurrentFrameIndex]->GetImageAvailableSemaphore().GetHandle();

		auto aquireResult = vkAcquireNextImageKHR(m_pDevice->GetHandle(), m_hSwapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &m_CurrentFrameIndex);
		
		if (aquireResult != VK_SUCCESS)
		{
			HZ_CORE_ERROR("present errors {0}, {1}", VulkanUtils::ResultToString(aquireResult).c_str(), VulkanUtils::ResultToString(aquireResult).c_str());
			HZ_CORE_ERROR("Attempting to recreate swapchain");
			auto extent = QueryCurrentExtent();
			InitSwapchain(extent.width, extent.height);

		}
		
		//VK_CHECK_RESULT(aquireResult, "Failed to aquire next image, current index is {0}", m_CurrentFrameIndex);

		return m_pFrameBuffers[m_CurrentFrameIndex];
	}

	void VulkanSwapChain::InitSwapchain(uint32_t width, uint32_t height)
	{
		VkPhysicalDevice hPhysicalDevice = m_pDevice->GetPhysicalDeviceHandle();
		VkSurfaceCapabilitiesKHR surfaceCapabilities = Vulkan::GetPhysicalDeviceSurfaceCapabilities(hPhysicalDevice, m_hSurface);
		VkSurfaceFormatKHR surfaceFormats = VulkanUtils::SelectSurfaceFormat(
			Vulkan::GetPhysicalDeviceSurfaceFormats(hPhysicalDevice, m_hSurface)
		);

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.surface = m_hSurface;
		createInfo.minImageCount = surfaceCapabilities.minImageCount;
		createInfo.imageFormat = surfaceFormats.format;
		createInfo.imageColorSpace = surfaceFormats.colorSpace;

		createInfo.imageExtent = VulkanUtils::SelectSurfaceExtent(
			{ m_Width, m_Height },
			surfaceCapabilities.currentExtent,
			surfaceCapabilities.minImageExtent,
			surfaceCapabilities.maxImageExtent);

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

		createInfo.preTransform = surfaceCapabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = VulkanUtils::SelectSurfacePresentMode(
			Vulkan::GetPhysicalDeviceSurfaceAvailablePresentModes(hPhysicalDevice, m_hSurface));
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = m_hSwapchain;

		VK_CHECK_RESULT(vkCreateSwapchainKHR(m_pDevice->GetHandle(), &createInfo, nullptr, &m_hSwapchain), "Failed to create a SwapChain");

		m_Width = width;
		m_Height = height;
		m_BackBuffersCount = createInfo.minImageCount;

		m_pFence = new VulkanFence(m_pDevice);

		InitRenderPass(createInfo.imageFormat);
		InitFrameBuffer(createInfo.imageFormat);
	}

	void VulkanSwapChain::InitRenderPass(VkFormat colorFormat)
	{
		std::vector<VkAttachmentDescription> attachmentsDesc;

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.flags			= 0;
		colorAttachment.format			= colorFormat;
		colorAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		attachmentsDesc.push_back(colorAttachment);

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		
		std::vector<VkSubpassDescription> subpassDesc;
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpassDesc.push_back(subpass);

		std::vector<VkSubpassDependency> subpassDependencies;

		m_pRenderPass = new VulkanRenderPass(m_pDevice, attachmentsDesc, subpassDesc, subpassDependencies);
	}


	VkExtent2D VulkanSwapChain::QueryCurrentExtent() const
	{
		return Vulkan::GetPhysicalDeviceSurfaceCapabilities(m_pDevice->GetPhysicalDeviceHandle(), m_hSurface).currentExtent;
	}

	void VulkanSwapChain::InitFrameBuffer(VkFormat format)
	{
		std::vector<VkImage> images = Vulkan::GetSwapchainImages(m_pDevice->GetHandle(), m_hSwapchain);

		for (auto image : images)
		{
			std::vector<VkImageView> views = {};
			VkImageViewCreateInfo viewCreateInfo			= {};
			viewCreateInfo.sType							= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.pNext							= nullptr;
			viewCreateInfo.flags							= 0;
			viewCreateInfo.image							= image;
			viewCreateInfo.viewType							= VK_IMAGE_VIEW_TYPE_2D;
			viewCreateInfo.format							= format;
			viewCreateInfo.components.r						= VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g						= VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b						= VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a						= VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel	= 0;
			viewCreateInfo.subresourceRange.levelCount		= 1;
			viewCreateInfo.subresourceRange.baseArrayLayer	= 0;
			viewCreateInfo.subresourceRange.layerCount		= 1;

			VkImageView view = VK_NULL_HANDLE;
			VK_CHECK_RESULT(vkCreateImageView(m_pDevice->GetHandle(), &viewCreateInfo, nullptr, &view), "Failed to create image view");
			views.push_back(view);
			m_pFrameBuffers.push_back(new VulkanFrameBuffer(m_pDevice, m_Width, m_Height, image, m_pRenderPass, views));
		}
	}

}
