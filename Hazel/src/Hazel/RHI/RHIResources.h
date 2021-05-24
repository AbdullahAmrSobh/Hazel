#pragma once
#include <string> 
#include <vector>
#include <cstdint>

namespace Hazel {

	enum class RHIResult
	{
		eSuccess,
		eFailed,
	};

	enum class RHIFormat
	{
		eSignedR32,
		eSignedRG32,
		eSignedRGB32,
		eSignedRGBA32,

		eSignedR16,
		eSignedRG16,
		eSignedRGB16,
		eSignedRGBA16,

		eSignedR8,
		eSignedRG8,
		eSignedRGB8,
		eSignedRGBA8,

		eUnsignedR32,
		eUnsignedRG32,
		eUnsignedRGB32,
		eUnsignedRGBA32,

		eUnsignedR16,
		eUnsignedRG16,
		eUnsignedRGB16,
		eUnsignedRGBA16,

		eUnsignedR8,
		eUnsignedRG8,
		eUnsignedRGB8,
		eUnsignedRGBA8,

		eFloatR,
		eFloatRG,
		eFloatRGB,
		eFloatRGBA,

		eDoubleR,
		eDoubleRG,
		eDoubleRGB,
		eDoubleRGBA,

		// Added 
		eSRGB_R8,
		eSRGB_RG8,
		eSRGB_RGB8,
		eSRGB_RGBA8,
	};

	enum class RHIFilter
	{
		eUndefined,
		eNearest,
		eLinear,
	};

	enum class RHITextureAddressMode
	{
		eUndefined,
		eRepeat,
		eMirroredRepeat,
		eClampToEdge,
		eClampToBorder,
		eMirroredClampToEdge,
	};

	enum class RHIComparsionOperation
	{
		eUndefined,
		eNever,
		eLess,
		eEqual,
		eLessOrEqual,
		eGreater,
		eNotEqual,
		eGreaterOrEqual,
		eAlways,
	};

	enum class RHISamplerBorderColor
	{
		eFloatTransparentBlack,
		eIntTransparentBlack,
		eFloatOpaqueBlack,
		eIntOpaqueBlack,
		eFloatOpaqueWhite,
		eIntOpaqueWhite,
		eUndefined
	};

	inline size_t GetFormatSize(RHIFormat format)
	{
		switch (format)
		{
		case RHIFormat::eSignedR32:			return 1 * sizeof(int32_t);
		case RHIFormat::eSignedRG32:		return 2 * sizeof(int32_t);
		case RHIFormat::eSignedRGB32:		return 3 * sizeof(int32_t);
		case RHIFormat::eSignedRGBA32:		return 4 * sizeof(int32_t);

		case RHIFormat::eSignedR16:			return 1 * sizeof(int16_t);
		case RHIFormat::eSignedRG16:		return 2 * sizeof(int16_t);
		case RHIFormat::eSignedRGB16:		return 3 * sizeof(int16_t);
		case RHIFormat::eSignedRGBA16:		return 4 * sizeof(int16_t);
		
		case RHIFormat::eSignedR8:			return 1 * sizeof(int8_t);
		case RHIFormat::eSignedRG8:			return 2 * sizeof(int8_t);
		case RHIFormat::eSignedRGB8:		return 3 * sizeof(int8_t);
		case RHIFormat::eSignedRGBA8:		return 4 * sizeof(int8_t);
		
		case RHIFormat::eUnsignedR32:		return 1 * sizeof(uint32_t);
		case RHIFormat::eUnsignedRG32:		return 2 * sizeof(uint32_t);
		case RHIFormat::eUnsignedRGB32:		return 3 * sizeof(uint32_t);
		case RHIFormat::eUnsignedRGBA32:	return 4 * sizeof(uint32_t);
															  
		case RHIFormat::eUnsignedR16:		return 1 * sizeof(uint16_t);
		case RHIFormat::eUnsignedRG16:		return 2 * sizeof(uint16_t);
		case RHIFormat::eUnsignedRGB16:		return 3 * sizeof(uint16_t);
		case RHIFormat::eUnsignedRGBA16:	return 4 * sizeof(uint16_t);
															  
		case RHIFormat::eUnsignedR8:		return 1 * sizeof(uint8_t);
		case RHIFormat::eUnsignedRG8:		return 2 * sizeof(uint8_t);
		case RHIFormat::eUnsignedRGB8:		return 3 * sizeof(uint8_t);
		case RHIFormat::eUnsignedRGBA8:		return 4 * sizeof(uint8_t);

		case RHIFormat::eFloatR:			return 1 * sizeof(float);
		case RHIFormat::eFloatRG:			return 2 * sizeof(float);
		case RHIFormat::eFloatRGB:			return 3 * sizeof(float);
		case RHIFormat::eFloatRGBA:			return 4 * sizeof(float);

		case RHIFormat::eDoubleR:			return 1 * sizeof(double);
		case RHIFormat::eDoubleRG:			return 2 * sizeof(double);
		case RHIFormat::eDoubleRGB:			return 3 * sizeof(double);
		case RHIFormat::eDoubleRGBA:		return 4 * sizeof(double);
		
		default:							return SIZE_MAX;
		
		}
	}

	struct RHIColor
	{
		float R;
		float G;
		float B;
		float A;
	};

	struct RHIViewport
	{
		uint32_t	OffsetX;
		uint32_t	OffsetY;
		uint32_t	Width;
		uint32_t	Height;
		float		MinDepth;
		float		MaxDepth;
	};

	struct RHIReact2D
	{
		uint32_t OffsetX;
		uint32_t OffsetY;
		uint32_t Width;
		uint32_t Height;
	};

	struct RHISamplerDesc
	{
		RHIFilter					Filter;
		RHITextureAddressMode		AddressU;
		RHITextureAddressMode		AddressV;
		RHITextureAddressMode		AddressW;
		float						MipLODBias;
		uint32_t                    MaxAnisotropy;
		RHIComparsionOperation      ComparisonFunc;
		float						BorderColor[4];
		float						MinLOD;
		float						MaxLOD;
		RHISamplerBorderColor		Border;
	};

	struct RHITexture2DDesc
	{
		uint32_t	MipLevels;
		RHIFormat	Format;
		uint32_t	Width;
		uint32_t	Height;
		uint32_t	SampleCount;
		uint32_t	SampleQaulity;
	};

	struct RHIVertexAttribute
	{
		std::string		Name;
		RHIFormat		Format;
	};

	struct RHIVertexBufferLayout
	{
		RHIVertexBufferLayout(const std::initializer_list<RHIVertexAttribute> attributes)
			: Attributes(attributes), Stride(0)
		{
			for (auto a : attributes) Stride += GetFormatSize(a.Format);
		}

		std::vector<RHIVertexAttribute> Attributes;
		size_t Stride;
	};

	class RHIBuffer
	{
	public:

		inline size_t GetSize() const { return m_BufferSize; }

	protected:
		size_t m_BufferSize;

	};

	class RHITexture 
	{ 
	public:
		inline size_t GetSize() const { return m_ImageSize; }

	protected:
		size_t m_ImageSize;

	};

	class RHISampler
	{

	};

	class RHITexture2D  
	{ 
	public:
		RHITexture2D(uint32_t width, uint32_t height) 
			: m_Width(width)
			, m_Height(height)
		{}

		inline RHITexture* GetTextureResource() const { return m_pTexture; }

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

	protected:
		uint32_t	m_Width;
		uint32_t	m_Height;
		RHITexture* m_pTexture = nullptr;
	};

	class RHIBufferResourceBase
	{
	public:
		inline RHIBuffer* GetBuffer() const { return m_pBuffer; }

	protected:
		RHIBuffer* m_pBuffer;

	};

	class RHIStagingBuffer : public RHIBufferResourceBase
	{
	public:
		virtual void* Lock(size_t offset, size_t size) = 0;
		virtual void Unlock() = 0;

	};

	class RHIVertexBuffer : public RHIBufferResourceBase
	{
	public:
		inline uint32_t GetVertexCount() const { return m_VertexCount; };
		inline uint32_t GetStride() const { return m_GetStride; }
		
	protected:
		uint32_t m_VertexCount;
		uint32_t m_GetStride;
	
	};

	class RHIIndexBuffer : public RHIBufferResourceBase
	{
	public:
		inline uint32_t GetIndciesCount() const { return m_IndciesCount; };
		inline uint32_t GetStride() const { return m_GetStride; }

	protected:
		uint32_t m_IndciesCount;
		uint32_t m_GetStride;
	};

	class RHIUniformBuffer : public RHIBufferResourceBase
	{
	public:
		virtual void SetData(size_t dataSize, const void* pData) = 0;

	};

	class RHIFence
	{
	public:
		virtual void Reset() = 0;
		virtual void Wait() = 0;
		virtual bool GetFenceStatus() = 0;

	};

}
