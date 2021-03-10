#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>

namespace Hazel {

	class RHIPrecompiledShader
	{
	public:
		RHIPrecompiledShader(size_t size, std::byte* pData)
		{
			m_Data.resize(size);
			memcpy(m_Data.data(), pData, size);
		}

		inline size_t GetSize() const { return m_Data.size(); }
		inline const std::byte* GetData() const { return m_Data.data(); }
		inline std::byte* GetData() { return m_Data.data(); }

	private:
		std::vector<std::byte> m_Data;
	};

	enum class RHIShaderType
	{
		eVertex,
		ePixel,
	};

	class RHIShader
	{
	public:
		virtual ~RHIShader() = default;

	private:
		std::string m_EntryFunctionName;
		RHIShaderType m_Type;
	};
}
