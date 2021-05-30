#pragma once
#include "Hazel/RHI/RHIShader.h"

#include <vulkan/vulkan.h>
#include <shaderc/shaderc.hpp>

namespace Hazel {

	class VulkanDevice;

	namespace ShadercUtils
	{
		static shaderc_shader_kind ConvertToShadercType(RHIShaderType type)
		{
			switch (type)
			{
			case RHIShaderType::eVertex:
				return shaderc_vertex_shader;
			case RHIShaderType::ePixel:
				return shaderc_fragment_shader;
			}

			HZ_CORE_ASSERT(false, "unkowen or unimplemented shader type");
			return shaderc_vertex_shader;
		}

	}

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

		virtual RHIShader* CompileFromSource(const std::string& source, RHIShaderType type, std::string& nputFilePath) const final override;
		virtual RHIShader* CompileShader(const std::vector<std::byte>& bin) const final override;



	private:
		const VulkanDevice* m_pDevice;
		std::string			m_InputFilePath;
		shaderc::Compiler	m_Compiler;
	};


}
