#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanMemory.h"
#include "Platform/VulkanRHI/VulkanResources.h"


namespace Hazel {

	VulkanSampler::VulkanSampler(const VulkanDevice* pDevice, const VkSamplerCreateInfo& createInfo)
		: m_pDevice(pDevice)
		, m_hSampler(VK_NULL_HANDLE)
	{
		VK_CHECK_RESULT(vkCreateSampler(m_pDevice->GetHandle(), &createInfo, nullptr, &m_hSampler), "Failed to create a sampler object");
	}

	VulkanSampler::~VulkanSampler()
	{
		vkDestroySampler(m_pDevice->GetHandle(), m_hSampler, nullptr);
	}

	VulkanImage::VulkanImage(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator)
		: m_pDevice(pDevice)
		, m_pAllocator(pAllocator)
		, m_hImage(VK_NULL_HANDLE)
		, m_Allocation(VK_NULL_HANDLE)
		, m_AllocationInfo({})
	{

	}

	VulkanImage::VulkanImage(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, const VkImageCreateInfo& createInfo, const VmaAllocationCreateInfo& allocInfo)
		: m_pDevice(pDevice)
		, m_pAllocator(pAllocator)
		, m_hImage(VK_NULL_HANDLE)
		, m_Allocation(VK_NULL_HANDLE)
		, m_AllocationInfo({}) 
	{
		Init(createInfo, allocInfo);
	}

	VulkanImage::~VulkanImage()
	{
		vmaDestroyImage(m_pAllocator->GetHandle(), m_hImage, m_Allocation);
	}

	void VulkanImage::Init(const VkImageCreateInfo& createInfo, const VmaAllocationCreateInfo& allocationInfo)
	{
		VK_CHECK_RESULT(vmaCreateImage(m_pAllocator->GetHandle(), &createInfo, &allocationInfo, &m_hImage, &m_Allocation, &m_AllocationInfo),
			"Failed to create image");
	}

	VulkanTexture2D::VulkanTexture2D(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, const RHITexture2DDesc& desc)
		: RHITexture2D(desc.Width, desc.Height)
		, m_pDevice(pDevice)
		, m_pAllocator(pAllocator)
	{
		{
			VkImageCreateInfo createInfo		= {};
			createInfo.sType					= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			createInfo.pNext					= nullptr;
			createInfo.flags					= 0;
			createInfo.imageType				= VK_IMAGE_TYPE_2D;
			createInfo.format					= VulkanUtils::GetFromat(desc.Format);
			createInfo.extent					= { desc.Width, desc.Height, 1 };
			createInfo.mipLevels				= 1;
			createInfo.arrayLayers				= 1;
			createInfo.samples					= VulkanUtils::GetSampleCount(desc.SampleCount);
			createInfo.tiling					= VK_IMAGE_TILING_OPTIMAL;
			createInfo.usage					= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			createInfo.sharingMode				= VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount	= 0;
			createInfo.pQueueFamilyIndices		= nullptr;
			createInfo.initialLayout			= VK_IMAGE_LAYOUT_UNDEFINED;

			VmaAllocationCreateInfo allocationInfo = {};
			allocationInfo.flags;
			allocationInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			allocationInfo.preferredFlags ;
			allocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			m_pTexture = new VulkanImage(m_pDevice, m_pAllocator, createInfo, allocationInfo);
		}

		VulkanImage* pVulkanTexture = reinterpret_cast<VulkanImage*>(m_pTexture);
		{
			VkImageViewCreateInfo createInfo			= {};
			createInfo.sType							= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.pNext							= nullptr;
			createInfo.flags							= 0;
			createInfo.image							= pVulkanTexture->GetHandle();;
			createInfo.viewType							= VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format							= VulkanUtils::GetFromat(desc.Format);
			createInfo.components.r						= VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g						= VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b						= VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a						= VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel	= 0;
			createInfo.subresourceRange.levelCount		= 1;
			createInfo.subresourceRange.baseArrayLayer	= 0;
			createInfo.subresourceRange.layerCount		= 1;

			VK_CHECK_RESULT(vkCreateImageView(m_pDevice->GetHandle(), &createInfo, nullptr, &m_hView), "Failed to create a view for the image");
		}
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
		vkDestroyImageView(m_pDevice->GetHandle(), m_hView, nullptr);
	}


}
