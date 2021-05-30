#include "hzpch.h"
#include <shaderc/shaderc.hpp>

#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanShader.h"

namespace Hazel {


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

	RHIShader* VulkanShaderCompiler::CompileFromSource(const std::string& source, RHIShaderType type, std::string& nputFilePath) const
	{
		const char* entryPoint = "main";

		auto result = m_Compiler.CompileGlslToSpv(source, ShadercUtils::ConvertToShadercType(type), nputFilePath.c_str());
		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			HZ_CORE_ERROR(result.GetErrorMessage());
			HZ_CORE_ASSERT(false);
		}

		size_t size = 0;
		for (auto data : result)
		{
			size += sizeof(data);
		}

		return new VulkanShader(m_pDevice, size, result.cbegin(), entryPoint, type);
	}

	RHIShader* VulkanShaderCompiler::CompileShader(const std::vector<std::byte>& bin) const
	{
		return new VulkanShader(m_pDevice, bin.size(), reinterpret_cast<const uint32_t*>(bin.data()), "main", RHIShaderType::eNone);
	}


}
