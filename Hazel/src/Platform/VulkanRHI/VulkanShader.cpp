#include "hzpch.h"
#include <shaderc/shaderc.hpp>

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanShader.h"

namespace Hazel {

	shaderc_shader_kind ConvertToShadercType(RHIShaderType type)
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

	VulkanShader::VulkanShader(const VulkanDevice* pDevice, size_t size, const uint32_t* pData, std::string name, RHIShaderType type)
		: RHIShader(size, reinterpret_cast<const std::byte*>(pData), type)
		, m_pDevice(pDevice)
		, m_Name(name)
		, m_Handle(VK_NULL_HANDLE)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.codeSize = size;
		createInfo.pCode = pData;

		VK_CHECK_RESULT(vkCreateShaderModule(m_pDevice->GetHandle(), &createInfo, nullptr, &m_Handle), "Failed to create a shader module");
	}

	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(m_pDevice->GetHandle(), m_Handle, nullptr);
	}

	VulkanShaderCompiler::VulkanShaderCompiler(const VulkanDevice* pDevice)
		: m_pDevice(pDevice)
	{
	}


	RHIShader* VulkanShaderCompiler::CompileFromSource(const std::string& source, RHIShaderType type) const
	{
		// auto res = m_Compiler.CompileGlslToSpv(source, ConvertToShadercType(type), "main");
		// 
		// if (res.GetCompilationStatus() != shaderc_compilation_status_success)
		// {
		// 	HZ_CORE_ERROR("Failed to compile {0} errors found, {1}", res.GetNumErrors(), res.GetErrorMessage());
		// 	HZ_CORE_WARN("{0} Warning found, {1}", res.GetNumWarnings());
		// 
		// 	return nullptr;
		// }
		// 
		// std::vector<const uint32_t*> code{ res.begin(), res.end() };
		// 
		// return new VulkanShader(m_pDevice, code.size(), (uint32_t*)code.data(), type);

		return nullptr;
	}

	RHIShader* VulkanShaderCompiler::CompileShader(const std::vector<std::byte>& bin) const
	{
		return new VulkanShader(m_pDevice, bin.size(), reinterpret_cast<const uint32_t*>(bin.data()), "main", RHIShaderType::eNone);
	}


}
