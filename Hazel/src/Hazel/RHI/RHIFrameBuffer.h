#pragma once
#include <cstdint>

namespace Hazel {

	enum class RHIFormat
	{
		eR8_Uint,
		eRG8_Uint,
		eRGB8_Uint,
		eRGBA8_uint,

		eR8_SInt,
		eRG8_SInt,
		eRGB8_SInt,
		eRGBA8_SInt,

		eR16_Uint,
		eRG16_Uint,
		eRGB16_Uint,
		eRGBA16_uint,

		eR16_SInt,
		eRG16_SInt,
		eRGB16_SInt,
		eRGBA16_SInt,

		eR32_Uint,
		eRG32_Uint,
		eRGB32_Uint,
		eRGBA32_uint,

		eR32_SInt,
		eRG32_SInt,
		eRGB32_SInt,
		eRGBA32_SInt,

		eR32_Float,
		eRG32_Float,
		eRGB32_Float ,
		eRGBA32_Float,

		eR64_Double,
		eRG64_Double,
		eRGB64_Double,
		eRGBA64_Double,

	};

	inline static uint32_t GetFormatSize(RHIFormat format)
	{
		switch (format)
		{
		case RHIFormat::eR8_Uint: return 1 * sizeof(uint8_t);
		case RHIFormat::eRG8_Uint: return 2 * sizeof(uint8_t);
		case RHIFormat::eRGB8_Uint: return 3 * sizeof(uint8_t);
		case RHIFormat::eRGBA8_uint: return 4 * sizeof(uint8_t);

		case RHIFormat::eR8_SInt: return 1 * sizeof(int8_t);
		case RHIFormat::eRG8_SInt: return 2 * sizeof(int8_t);
		case RHIFormat::eRGB8_SInt: return 3 * sizeof(int8_t);
		case RHIFormat::eRGBA8_SInt: return 4 * sizeof(int8_t);

		case RHIFormat::eR16_Uint: return 1 * sizeof(uint16_t);
		case RHIFormat::eRG16_Uint: return 2 * sizeof(uint16_t);
		case RHIFormat::eRGB16_Uint: return 3 * sizeof(uint16_t);
		case RHIFormat::eRGBA16_uint: return 4 * sizeof(uint16_t);

		case RHIFormat::eR16_SInt: return 1 * sizeof(int16_t);
		case RHIFormat::eRG16_SInt: return 2 * sizeof(int16_t);
		case RHIFormat::eRGB16_SInt: return 3 * sizeof(int16_t);
		case RHIFormat::eRGBA16_SInt: return 4 * sizeof(int16_t);

		case RHIFormat::eR32_Uint: return 1 * sizeof(uint32_t);
		case RHIFormat::eRG32_Uint: return 2 * sizeof(uint32_t);
		case RHIFormat::eRGB32_Uint: return 3 * sizeof(uint32_t);
		case RHIFormat::eRGBA32_uint: return 4 * sizeof(uint32_t);

		case RHIFormat::eR32_SInt: return 1 * sizeof(int32_t);
		case RHIFormat::eRG32_SInt: return 2 * sizeof(int32_t);
		case RHIFormat::eRGB32_SInt: return 3 * sizeof(int32_t);
		case RHIFormat::eRGBA32_SInt: return 3 * sizeof(int32_t);

		case RHIFormat::eR32_Float: return 1 * sizeof(float);
		case RHIFormat::eRG32_Float: return 2 * sizeof(float);
		case RHIFormat::eRGB32_Float: return 3 * sizeof(float);
		case RHIFormat::eRGBA32_Float: return 4 * sizeof(float);

		case RHIFormat::eR64_Double: return 1 * sizeof(double);
		case RHIFormat::eRG64_Double: return 2 * sizeof(double);
		case RHIFormat::eRGB64_Double: return 3 * sizeof(double);
		case RHIFormat::eRGBA64_Double: return 4 * sizeof(double);
		default: return 0;
		}
	}

	class RHIFrameBuffer
	{
	public:
		virtual ~RHIFrameBuffer() = default;
		
		inline uint32_t Width() const { return 0; };
		inline uint32_t Height() const { return 0; };
	
	protected:
		uint32_t m_Width, m_Height;

	};
}
