#pragma once
#include <cstddef>
#include <vector>
#include <initializer_list>

#include "Hazel/RHI/RHIShader.h"

namespace Hazel {

	enum class RHIPipelineShaderStage
	{
		eVertexShader,
		ePixelShader,
	};

	enum class RHIDescriptorType
	{
		eUniformBuffer,
		eTexture
	};

	struct RHIDescriptorSetBindingDesc
	{
		std::string				BindingName;
		uint32_t				Binding;
		RHIDescriptorType		Type;
		uint32_t				Count;
		RHIPipelineShaderStage	Stage;
	};

	struct RHIDescriptorSetLayoutDesc
	{
		std::string									LayoutName;
		std::vector<RHIDescriptorSetBindingDesc>	LayoutBindings;
	};

	struct RHIPipelineLayoutDesc
	{
		std::vector<RHIDescriptorSetLayoutDesc> DescriptorsLayouts;
	};

	class RHIDescriptorPool
	{
	public:

		inline uint32_t GetMaxDescriptorSetsCount() const { return m_MaxDescriptorSetsCount; }

		virtual class RHIDescriptorSet* AllocateDescriptorSet(const RHIDescriptorSetLayoutDesc& layout) = 0;

	protected:
		uint32_t m_MaxDescriptorSetsCount;
	
	};

	class RHIDescriptorSet
	{
	public:
		virtual void BindUiformBuffer(uint32_t binding, const class RHIUniformBuffer* pBuffer) = 0;
		virtual void BindTexture(uint32_t binding, const class RHITexture2D* pTexture, const class RHISampler* pSampler) = 0;

	};

}
