#include "hzpch.h"

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanQueue.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"
#include "Platform/VulkanRHI/VulkanResources.h"
#include "Platform/VulkanRHI/VulkanSwapChain.h"

namespace Hazel {


	VulkanSwapChain::VulkanSwapChain(const VulkanDevice* pDevice, const VulkanQueue* pQueue, const VkSwapchainCreateInfoKHR& createInfo)
		: m_pDevice(pDevice)
		, m_pQueue(pQueue)
		, m_SwapChainHandle(VK_NULL_HANDLE)
		, m_pRenderPass(nullptr)
		, m_Width(createInfo.imageExtent.width)
		, m_Height(createInfo.imageExtent.height)
	{
		VK_CHECK_RESULT(vkCreateSwapchainKHR(m_pDevice->GetHandle(), &createInfo, nullptr, &m_SwapChainHandle), "Failed to create a SwapChain");

		VkFormat format = createInfo.imageFormat;

		m_CurrentFrameIndex = 0;

		RetriveTheSwapChainImages(format);
		InitRenderPass(format);
		InitFrameBuffer(format);
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		vkDestroySwapchainKHR(m_pDevice->GetHandle(), m_SwapChainHandle, nullptr);
	}

	void VulkanSwapChain::OnResize(uint32_t width, uint32_t height)
	{
		HZ_CORE_INFO("This method is not implemented yet");
	}

	void VulkanSwapChain::Present()
	{
		HZ_CORE_INFO("Failed to present");

		std::vector<VkSemaphore> renderingSemaphores = {};

		static VkResult presentationResult = VK_RESULT_MAX_ENUM;

		VkPresentInfoKHR presentInfo	= {};
		presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext				= nullptr;
		presentInfo.waitSemaphoreCount	= renderingSemaphores.size();			
		presentInfo.pWaitSemaphores		= renderingSemaphores.data();
		presentInfo.swapchainCount		= 1;
		presentInfo.pSwapchains			= &m_SwapChainHandle;
		presentInfo.pImageIndices		= &m_CurrentFrameIndex;
		presentInfo.pResults			= &presentationResult;

		static VkResult submitResult = vkQueuePresentKHR(m_pQueue->GetHandle(), &presentInfo);

		VK_CHECK_RESULT(submitResult, "Failed to submit to presentation queue");
		VK_CHECK_RESULT(presentationResult, "Failed to present");
	}

	void VulkanSwapChain::SwapBuffers()
	{
		HZ_CORE_INFO("This method is not implemented yet");
	}

	RHIFrameBuffer** VulkanSwapChain::GetFrameBuffer()
	{
		return reinterpret_cast<RHIFrameBuffer**>(m_pFrameBuffers.data());
	}

	void VulkanSwapChain::RetriveTheSwapChainImages(VkFormat imageFormat)
	{
		vkGetSwapchainImagesKHR(m_pDevice->GetHandle(), m_SwapChainHandle, &m_BackBuffersCount, nullptr);
		m_SwapChainImages.resize(m_BackBuffersCount);
		vkGetSwapchainImagesKHR(m_pDevice->GetHandle(), m_SwapChainHandle, &m_BackBuffersCount, m_SwapChainImages.data());
	}

	void VulkanSwapChain::InitRenderPass(VkFormat colorFormat)
	{
		std::vector<VkAttachmentDescription> attachmentsDesc;

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.flags = 0;
		colorAttachment.format = colorFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
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

	void VulkanSwapChain::InitFrameBuffer(VkFormat format)
	{
		for (auto image : m_SwapChainImages)
		{
			std::vector<VkImageView> views = {};
			VkImageView view = VK_NULL_HANDLE;
			VkImageViewCreateInfo viewCreateInfo = {};
			viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.pNext = nullptr;
			viewCreateInfo.flags = 0;
			viewCreateInfo.image = image;
			viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewCreateInfo.format = format;
			viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.subresourceRange;
			viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel = 0;
			viewCreateInfo.subresourceRange.levelCount = 1;
			viewCreateInfo.subresourceRange.baseArrayLayer = 0;
			viewCreateInfo.subresourceRange.layerCount = 1;

			VK_CHECK_RESULT(vkCreateImageView(m_pDevice->GetHandle(), &viewCreateInfo, nullptr, &view), "Failed to create image view");
			views.push_back(view);
			m_pFrameBuffers.push_back(new VulkanFrameBuffer(m_pDevice, m_Width, m_Height, image, m_pRenderPass, views));
		}
	}

}
