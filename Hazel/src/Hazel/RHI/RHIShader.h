#pragma once
#include "Hazel/RHI/RHIObject.h"

namespace Hazel {

	enum class RHIShaderType
	{
		eNone,
		eVertex,
		ePixel,
	};

	class RHIShader
	{
	public:
		RHIShader(size_t size, const std::byte* pCode, RHIShaderType type)
			: m_Type(type)
		{
			m_Code.resize(size);
			std::memcpy(m_Code.data(), pCode, size);
		}

		inline size_t GetSize() const { return m_Code.size(); }

		inline std::byte* GetData() { return m_Code.data(); }
		inline const std::byte* GetData() const { return m_Code.data(); }

		inline RHIShaderType GetType() const { return m_Type; }

	protected:
		std::vector<std::byte> m_Code;
		RHIShaderType m_Type;

	};

	class RHIVertexShader : public RHIShader { };
	class RHIPixelShader : public RHIShader { };

	class RHIShaderCompiler
	{
	public:
		virtual RHIShader* CompileFromSource(const std::string& source, RHIShaderType type) const = 0;
		virtual RHIShader* CompileShader(const std::vector<std::byte>& binary) const = 0;

		template<class TShaderType>
		TShaderType* CompileShader(const std::vector<std::byte>& binary) const
		{
			return reinterpret_cast<TShaderType*>(CompileShader(pData));
		}

	};

}
