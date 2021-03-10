#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanSwapChain.h"

namespace Hazel {

	VulkanSwapChain::VulkanSwapChain(const VulkanDevice* pDevice, const VulkanSwapChainDesc& desc)
		: m_pDevice(pDevice), m_SwapChainHandle(VK_NULL_HANDLE), m_RenderPass(VulkanRenderPass(pDevice))
	{
		VkSwapchainCreateInfoKHR vkSwapchainCreateInfo	= {};
		vkSwapchainCreateInfo.sType						= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		vkSwapchainCreateInfo.pNext						= NULL;
		vkSwapchainCreateInfo.flags						= {};
		vkSwapchainCreateInfo.surface					= desc.surface;
		vkSwapchainCreateInfo.minImageCount				= desc.minImageCount;
		vkSwapchainCreateInfo.imageFormat				= desc.surfaceFormat.format;
		vkSwapchainCreateInfo.imageColorSpace			= desc.surfaceFormat.colorSpace;
		vkSwapchainCreateInfo.imageExtent				= desc.swapcainExtent;
		vkSwapchainCreateInfo.imageArrayLayers			= 1;
		vkSwapchainCreateInfo.imageUsage				= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		vkSwapchainCreateInfo.imageSharingMode			= VK_SHARING_MODE_EXCLUSIVE;
		vkSwapchainCreateInfo.queueFamilyIndexCount		= 0;	
		vkSwapchainCreateInfo.pQueueFamilyIndices		= nullptr; // Check on this later
		vkSwapchainCreateInfo.preTransform				= desc.transform;
		vkSwapchainCreateInfo.compositeAlpha			= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		vkSwapchainCreateInfo.presentMode				= desc.presentMode;
		vkSwapchainCreateInfo.clipped					= VK_TRUE;
		VK_ASSERT(vkCreateSwapchainKHR(m_pDevice->GetHandle(), &vkSwapchainCreateInfo, nullptr, &m_SwapChainHandle), "Failed to create or recreate the swapchain.");
		
		// Obtain VkImages for the framebuffer
		uint32_t swapchainImagesCount;
		VK_ASSERT(vkGetSwapchainImagesKHR(m_pDevice->GetHandle(), m_SwapChainHandle, &swapchainImagesCount, nullptr), "Failed to obtain the count of swapchain images");
		std::vector<VkImage> imagesBuffer;  
		imagesBuffer.resize(swapchainImagesCount);
		VK_ASSERT(vkGetSwapchainImagesKHR(m_pDevice->GetHandle(), m_SwapChainHandle, &swapchainImagesCount, imagesBuffer.data()), "Failed to aquire the swapchain images");

		VulkanRenderPassDesc renderPassDesc			= {};
		VkAttachmentDescription colorAttachmentDesc = {};
		colorAttachmentDesc.flags					= 0;
		colorAttachmentDesc.format					= desc.surfaceFormat.format;
		colorAttachmentDesc.samples					= VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentDesc.loadOp					= VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachmentDesc.storeOp					= VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentDesc.stencilLoadOp			= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentDesc.stencilStoreOp			= VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentDesc.initialLayout			= VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentDesc.finalLayout				= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		renderPassDesc.AttachmentDesc.push_back(colorAttachmentDesc);

		VkAttachmentReference colorAttachmentReference	= {};
		colorAttachmentReference.attachment				= 0;
		colorAttachmentReference.layout					= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		renderPassDesc.AttachmentReferences.push_back(colorAttachmentReference);

		VkSubpassDescription colorSubPassDesc		= {};
		colorSubPassDesc.flags						= 0;
		colorSubPassDesc.pipelineBindPoint			= VK_PIPELINE_BIND_POINT_GRAPHICS;
		colorSubPassDesc.inputAttachmentCount		= 0;
		colorSubPassDesc.pInputAttachments			= nullptr;
		colorSubPassDesc.colorAttachmentCount		= renderPassDesc.AttachmentReferences.size();
		colorSubPassDesc.pColorAttachments			= renderPassDesc.AttachmentReferences.data();
		colorSubPassDesc.pResolveAttachments		= 0;
		colorSubPassDesc.pDepthStencilAttachment	= nullptr;
		colorSubPassDesc.preserveAttachmentCount	= 0;
		colorSubPassDesc.pPreserveAttachments		= nullptr;
		renderPassDesc.SubPassDesc.push_back(colorSubPassDesc);
		
		m_RenderPass.Init(renderPassDesc);
		
		VulkanFrameBufferDesc frameBufferDesc = {};
		frameBufferDesc.pRenderPass = &m_RenderPass;
		frameBufferDesc.Width = desc.swapcainExtent.width;
		frameBufferDesc.Height = desc.swapcainExtent.height;
		frameBufferDesc.Layers = 1;

		// TODO
		// I should instead create Copy, And move constructors for the VulkanFrameBuffers
		// Instead of doing this
		m_FrameBuffers.reserve(imagesBuffer.size());

		for (auto& image : imagesBuffer)
		{
			std::vector<VkImageView> views;
			views.resize(1);

			VkImageViewCreateInfo viewCI			= {};
			viewCI.sType							= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCI.pNext							= nullptr;
			viewCI.flags							= 0;
			viewCI.image							= image;
			viewCI.viewType							= VK_IMAGE_VIEW_TYPE_2D;
			viewCI.format							= desc.surfaceFormat.format;
			viewCI.components.r						= VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCI.components.g						= VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCI.components.b						= VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCI.components.a						= VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCI.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			viewCI.subresourceRange.baseMipLevel	= 0;
			viewCI.subresourceRange.levelCount		= 1;
			viewCI.subresourceRange.baseArrayLayer	= 0;
			viewCI.subresourceRange.layerCount		= 1;

			VK_ASSERT(vkCreateImageView(m_pDevice->GetHandle(), &viewCI, nullptr, views.data()),
				"Failed to create a image view for the color attachment");

			frameBufferDesc.ImageHandle = image;
			frameBufferDesc.Views		= views;

			m_FrameBuffers.emplace_back(m_pDevice, frameBufferDesc);
		}
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		vkDestroySwapchainKHR(m_pDevice->GetHandle(), m_SwapChainHandle, nullptr);
	}

	void VulkanSwapChain::SwapFrameBuffers()
	{
		VK_ASSERT(vkAcquireNextImageKHR(m_pDevice->GetHandle(), m_SwapChainHandle, UINT64_MAX, m_ImageAvailableSemaphore, VK_NULL_HANDLE, &m_CurrentFrameBufferIndex)
			, "Failed to aquire the next image of index [{0}]", m_CurrentFrameBufferIndex);
	}

	void VulkanSwapChain::Present()
	{
		VkResult presentationResult;
		VkPresentInfoKHR presentInfo	= {};
		presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext				= nullptr;
		presentInfo.waitSemaphoreCount	= m_WaitSemaphoresHandles.size();
		presentInfo.pWaitSemaphores		= m_WaitSemaphoresHandles.data();
		presentInfo.swapchainCount		= 1;
		presentInfo.pSwapchains			= &m_SwapChainHandle;
		presentInfo.pImageIndices		= &m_CurrentFrameBufferIndex;
		presentInfo.pResults			= &presentationResult;

		VkQueue queueHandle = m_pDevice->GetPresentQueue().GetHandle();

		VK_ASSERT(vkQueuePresentKHR(queueHandle, &presentInfo), "SwapChain failed to present");
		VK_ASSERT(presentationResult, "Presentation failed");
	}

	uint32_t VulkanSwapChain::GetCurrentFrameBufferIndex()
	{
		return m_CurrentFrameBufferIndex;
	}

	uint32_t VulkanSwapChain::GetBackBuffersCount()
	{
		return m_FrameBuffers.size();
	}

	RHIFrameBuffer** VulkanSwapChain::GetFrameBuffers()
	{
		return reinterpret_cast<RHIFrameBuffer**>(m_FrameBuffers.data());
	}


}
