#pragma once

#include "Hazel/RHI/RHIDescriptors.h"

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"

#include <vulkan/vulkan.h>
#include <map>

namespace Hazel {
	
	class VulkanDevice;

	namespace VulkanUtils
	{
		static inline VkShaderStageFlags GetShaderStage(RHIPipelineShaderStage stage)
		{
			return VK_SHADER_STAGE_ALL;
		}

		static inline VkDescriptorType GetDescriptorType(RHIDescriptorType type)
		{
			switch (type)
			{
			case Hazel::RHIDescriptorType::eUniformBuffer:
				return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			}

			HZ_CORE_ERROR("Unkowen descriptor type");

			return VK_DESCRIPTOR_TYPE_MAX_ENUM;
		}

		static inline VkDescriptorPoolSize CalculateDescriptorPoolSize(const RHIDescriptorSetBindingDesc& binding)
		{
			VkDescriptorPoolSize size	= {};
			size.type					= GetDescriptorType(binding.Type);
			size.descriptorCount		= binding.Count;
			return size;
		}

		static inline VkDescriptorSetLayoutBinding ConvertToDSLBinding(const RHIDescriptorSetBindingDesc& bindingSlot)
		{
			VkDescriptorSetLayoutBinding binding	= {};
			binding.binding							= bindingSlot.Binding;
			binding.descriptorType					= GetDescriptorType(bindingSlot.Type);
			binding.descriptorCount					= bindingSlot.Count;
			binding.stageFlags						= GetShaderStage(bindingSlot.Stage);
			binding.pImmutableSamplers				= nullptr;

			return binding;
		}

		static inline size_t CalculateHash(const std::vector<std::string>& strings)
		{
			std::hash<std::string> hasher{};

			auto combineHash = [&](size_t base, const std::string& string)
			{
				size_t seed = base;
				seed ^= hasher(string) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				return seed;
			};

			size_t seed = 0;
		
			for (const auto& string : strings) seed += combineHash(seed, string);

			return seed;
		}

		static inline size_t CalculateDSLBindingHash(const RHIDescriptorSetLayoutDesc& dslDesc)
		{
			std::vector<std::string> bindingNames;
			bindingNames.reserve(dslDesc.LayoutBindings.size());

			for (const auto& binding : dslDesc.LayoutBindings)
				bindingNames.push_back(binding.BindingName);

			size_t key = VulkanUtils::CalculateHash(bindingNames);

			return key;
		}
	}

	class VulkanDescriptorsLayoutManager
	{
	public:
		VulkanDescriptorsLayoutManager(const VulkanDevice* pDevice);
		~VulkanDescriptorsLayoutManager();

		VkDescriptorSetLayout GetOrCreate(const RHIDescriptorSetLayoutDesc& dslDesc);

	private:
		const VulkanDevice* m_pDevice;
		std::unordered_map<size_t, VkDescriptorSetLayout> m_DescriptorsMap;
	};

	class VulkanDescriptorPool : public RHIDescriptorPool
	{
	public:
		VulkanDescriptorPool(
			const VulkanDevice* pDevice, 
			VulkanDescriptorsLayoutManager* manager,
			const std::vector<RHIDescriptorSetLayoutDesc>& layouts, 
			uint32_t maxSets = 1
		);
		~VulkanDescriptorPool();

		inline VkDescriptorPool GetHandle() const { return m_Handle; }
		inline uint32_t GetMaxSetsCount() const { return m_MaxSetsCount; }

		void Reset();

		virtual class RHIDescriptorSet* AllocateDescriptorSet(const RHIDescriptorSetLayoutDesc& layoutDesc) final override;

		VkDescriptorSet AllocateDescriptorSet(VkDescriptorSetLayout layout);
		void FreeDescriptorSet(VkDescriptorSet set);

	private:
		const VulkanDevice* m_pDevice;
		VulkanDescriptorsLayoutManager* m_pManager;
		VkDescriptorPool	m_Handle;
		uint32_t			m_MaxSetsCount;
	};

	class VulkanDescriptorSet : public RHIDescriptorSet
	{
	public:
		VulkanDescriptorSet(const VulkanDevice* pDevice, VulkanDescriptorPool* pPool, VkDescriptorSet set);
		~VulkanDescriptorSet();

		inline VkDescriptorSet GetHandle() const { return m_Handle; }

		virtual void BindUiformBuffer(uint32_t binding, const class RHIUniformBuffer* pBuffer) final override;

	private:
		const VulkanDevice* m_pDevice;
		VulkanDescriptorPool* m_pParantPool;
		VkDescriptorSet m_Handle;

	};
	
}
