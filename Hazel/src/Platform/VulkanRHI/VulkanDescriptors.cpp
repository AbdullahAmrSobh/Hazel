#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanResources.h"
#include "Platform/VulkanRHI/VulkanDescriptors.h"
#include "Platform/VulkanRHI/VulkanRHI.h"

namespace Hazel {

	RHIDescriptorPool* VulkanRHI::CreateDescriptorPool(const std::vector<RHIDescriptorSetLayoutDesc>& layouts, uint32_t maxSets)
	{
		return new VulkanDescriptorPool(m_pDevice, m_pDescriptorsLayoutManager, layouts, maxSets);
	}


	VulkanDescriptorsLayoutManager::VulkanDescriptorsLayoutManager(const VulkanDevice* pDevice)
		: m_pDevice(pDevice)
	{
	}

	VulkanDescriptorsLayoutManager::~VulkanDescriptorsLayoutManager()
	{
		for (const auto& dsl : m_DescriptorsMap) vkDestroyDescriptorSetLayout(m_pDevice->GetHandle(), dsl.second, nullptr);
	}

	VkDescriptorSetLayout VulkanDescriptorsLayoutManager::GetOrCreate(const RHIDescriptorSetLayoutDesc& dslDesc)
	{	
		size_t key = VulkanUtils::CalculateDSLBindingHash(dslDesc);

		auto result = m_DescriptorsMap.find(key);

		if (result == m_DescriptorsMap.end())
		{

			std::vector<VkDescriptorSetLayoutBinding> bindings;
			bindings.reserve(dslDesc.LayoutBindings.size());

			for (const auto& binding : dslDesc.LayoutBindings) bindings.push_back( VulkanUtils::ConvertToDSLBinding(binding) );
			
			VkDescriptorSetLayoutCreateInfo createInfo	= {};
			createInfo.sType							= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			createInfo.pNext							= nullptr;
			createInfo.flags							= 0;
			createInfo.bindingCount						= bindings.size();
			createInfo.pBindings						= bindings.data();

			VkDescriptorSetLayout layout = VK_NULL_HANDLE;
			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_pDevice->GetHandle(), &createInfo, nullptr, &layout), "Failed to create descriptor set layout");
			m_DescriptorsMap.emplace(std::make_pair(key, layout));
			return layout;
		}
		else
		{ 
			return result->second;
		}

	}

	VulkanDescriptorPool::VulkanDescriptorPool(
		const VulkanDevice* pDevice,
		VulkanDescriptorsLayoutManager* manager,
		const std::vector<RHIDescriptorSetLayoutDesc>& layouts,
		uint32_t maxSets
	)
		: m_pDevice(pDevice)
		, m_pManager(manager)
	{
		m_MaxDescriptorSetsCount = maxSets;
		
		std::vector<VkDescriptorPoolSize> sizes; 
		sizes.reserve(layouts.size());

		// this could be done with ranges
		for (const auto& layout : layouts)
			for (const auto& binding : layout.LayoutBindings) sizes.push_back(VulkanUtils::CalculateDescriptorPoolSize(binding));

		VkDescriptorPoolCreateInfo createInfo	= {};
		createInfo.sType						= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.pNext						= nullptr;
		createInfo.flags						= 0;
		createInfo.maxSets						= m_MaxDescriptorSetsCount;
		createInfo.poolSizeCount				= sizes.size();
		createInfo.pPoolSizes					= sizes.data();

		VK_CHECK_RESULT(vkCreateDescriptorPool(m_pDevice->GetHandle(), &createInfo, nullptr, &m_Handle), "Failed to create DescriptorPool");
	}
	
	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		vkDestroyDescriptorPool(m_pDevice->GetHandle(), m_Handle, nullptr);
	}

	void VulkanDescriptorPool::Reset()
	{
		vkResetDescriptorPool(m_pDevice->GetHandle(), m_Handle, 0);
	}

	RHIDescriptorSet* VulkanDescriptorPool::AllocateDescriptorSet(const RHIDescriptorSetLayoutDesc& layoutDesc)
	{
		VkDescriptorSetLayout dslLayout = m_pManager->GetOrCreate(layoutDesc);
		VkDescriptorSet setHandle = AllocateDescriptorSet(dslLayout);
		return new VulkanDescriptorSet(m_pDevice, this, setHandle);
	}

	VkDescriptorSet VulkanDescriptorPool::AllocateDescriptorSet(VkDescriptorSetLayout layout)
	{
		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.pNext = nullptr;
		allocateInfo.descriptorPool = m_Handle;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &layout;

		VkDescriptorSet set = VK_NULL_HANDLE;
		VK_CHECK_RESULT(vkAllocateDescriptorSets(m_pDevice->GetHandle(), &allocateInfo, &set), "Failed to allocate descriptor sets");
		return set;
	}

	void VulkanDescriptorPool::FreeDescriptorSet(VkDescriptorSet set)
	{
		VK_CHECK_RESULT(vkFreeDescriptorSets(m_pDevice->GetHandle(), m_Handle, 1, &set), "Failed to free descriptor set");
	}

	VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDevice* pDevice, VulkanDescriptorPool* pPool, VkDescriptorSet set)
		: m_pDevice(pDevice), m_pParantPool(pPool), m_Handle(set)
	{
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		m_pParantPool->FreeDescriptorSet(m_Handle);
	}

	void VulkanDescriptorSet::BindUiformBuffer(uint32_t binding, const RHIUniformBuffer* pBuffer)
	{
		const VulkanUniformBuffer* pVulkanUniformBuffer = reinterpret_cast<const VulkanUniformBuffer*>(pBuffer);
		const VulkanBuffer* pVulkanBuffer = reinterpret_cast<const VulkanBuffer*>(pVulkanUniformBuffer->GetBuffer());

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer	= pVulkanBuffer->GetHandle();
		bufferInfo.offset	= 0;
		bufferInfo.range	= pVulkanBuffer->GetSize();

		VkWriteDescriptorSet write	= {};
		write.sType					= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext					= nullptr;
		write.dstSet				= m_Handle;
		write.dstBinding			= binding;
		write.dstArrayElement		= 0;
		write.descriptorCount		= 1;
		write.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write.pBufferInfo			= &bufferInfo;

		vkUpdateDescriptorSets(m_pDevice->GetHandle(), 1, &write, 0, nullptr);
	}

	void VulkanDescriptorSet::BindTexture(uint32_t binding, const RHITexture2D* pTexture, const RHISampler* pSampler)
	{
		const VulkanTexture2D* pVulkanTexture2D = reinterpret_cast<const VulkanTexture2D*>(pTexture);
		const VulkanImage* pVulkanBuffer = reinterpret_cast<const VulkanImage*>(pVulkanTexture2D->GetTextureResource());
		const VulkanSampler* pVulkanSampler = reinterpret_cast<const VulkanSampler*>(pSampler);
		
		VkDescriptorImageInfo imageInfo		= {};
		imageInfo.sampler					= pVulkanSampler->GetHandle();
		imageInfo.imageLayout				= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView					= pVulkanTexture2D->GetHandle();

		VkWriteDescriptorSet write	= {};
		write.sType					= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext					= nullptr;
		write.dstSet				= m_Handle;
		write.dstBinding			= binding;
		write.dstArrayElement		= 0;
		write.descriptorCount		= 1;
		write.descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.pImageInfo			= &imageInfo;

		vkUpdateDescriptorSets(m_pDevice->GetHandle(), 1, &write, 0, nullptr);
	}



}
