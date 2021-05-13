#pragma once
#include "Hazel/RHI/RHIShader.h"

#include <vulkan/vulkan.h>
#include <shaderc/shaderc.hpp>

namespace Hazel {

	class VulkanDevice;

	shaderc_shader_kind ConvertToShadercType(RHIShaderType type);

	class VulkanShader : public RHIShader
	{
	public:
		VulkanShader(const VulkanDevice* pDevice, size_t size, const uint32_t* pData, std::string name, RHIShaderType type);
		~VulkanShader();
		
		inline VkShaderModule GetHandle() const { return m_Handle; }
		inline const std::string& GetName() const { return m_Name; }

	private:
		const VulkanDevice* m_pDevice;
		VkShaderModule m_Handle;
		std::string m_Name;
	};

	class VulkanShaderCompiler : public RHIShaderCompiler
	{
	public:
		VulkanShaderCompiler(const VulkanDevice* pDevice);

		virtual RHIShader* CompileFromSource(const std::string& source, RHIShaderType type) const final override;
		virtual RHIShader* CompileShader(const std::vector<std::byte>& bin) const final override;

	private:
		const VulkanDevice* m_pDevice;
		// shaderc::Compiler m_Compiler;
	};


}
