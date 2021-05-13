#pragma once
#include <string> 
#include <vector>
#include <cstdint>

namespace Hazel {

	using RHIColor = std::array<float, 4>;

	enum class RHIResult
	{
		eSuccess,
		eFailed,
	};

	enum class RHIImageFormat
	{
		eR32,
		eRG32,
		eRGB32,
		eRGBA32,
	};
	
	enum class RHIFormat
	{
		eFloat,
		eFloat2,
		eFloat3,
		eFloat4,

		eInt,
		eInt2,
		eInt3,
		eInt4,

		eUint,
		eUint2,
		eUint3,
		eUint4,
	};

	inline size_t GetSize(RHIFormat format)
	{

		switch (format)
		{
		case RHIFormat::eFloat:
			return 1 * sizeof(float);
		case RHIFormat::eFloat2:
			return 2 * sizeof(float);
		case RHIFormat::eFloat3:
			return 3 * sizeof(float);
		case Hazel::RHIFormat::eFloat4:
			return 4 * sizeof(float);
		
		case RHIFormat::eInt:
			return 1 * sizeof(int);
		case RHIFormat::eInt2:
			return 2 * sizeof(int);
		case RHIFormat::eInt3:
			return 3 * sizeof(int);
		case Hazel::RHIFormat::eInt4:
			return 4 * sizeof(int);

		case RHIFormat::eUint:
			return 1 * sizeof(uint32_t);
		case RHIFormat::eUint2:
			return 2 * sizeof(uint32_t);
		case RHIFormat::eUint3:
			return 3 * sizeof(uint32_t);
		case RHIFormat::eUint4:
			return 4 * sizeof(uint32_t);

		default:
			HZ_CORE_ASSERT("invalid size");
			return SIZE_MAX;
		}

		return SIZE_MAX;
	}

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
			for (auto a : attributes)
				Stride += GetSize(a.Format);
		}

		std::vector<RHIVertexAttribute> Attributes;
		size_t Stride;
	};

	struct RHIViewport
	{
		uint32_t OffsetX = 0;
		uint32_t OffsetY = 0;
		uint32_t Width;
		uint32_t Height;
		float MinDepth = 0.0f;
		float MaxDepth = 1.0f;
	};

	struct RHIReact2D
	{
		uint32_t OffsetX = 0;
		uint32_t OffsetY = 0;
		uint32_t Width;
		uint32_t Height;
	};

	class RHIBuffer : private NonCopyable
	{
	public:
		inline size_t GetSize() const { return m_BufferSize; }

	protected:
		size_t m_BufferSize;

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
		
		inline void CopyData(size_t srcSize, const void* pSrcData, size_t offset)
		{
			void* target = Lock(offset, srcSize);
			// uint32_t* pOffsetedTargetData = & reinterpret_cast<uint32_t*>(target)[offset];
			memcpy(target, pSrcData, srcSize);
			Unlock();
		}
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
