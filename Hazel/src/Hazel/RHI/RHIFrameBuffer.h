#pragma once

namespace Hazel {

	class RHIRenderPass
	{
	public:

		inline uint32_t GetAttachmentsCount() const { return m_AttachmentsCount; }

	private:
		uint32_t m_AttachmentsCount;
	};

	class RHIFrameBuffer 
	{
	public:
		RHIFrameBuffer(uint32_t width, uint32_t height, uint32_t frameIndex) 
			: m_Width(width), m_Height(height), m_FrameBufferIndex(frameIndex) 
		{ }

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }
		inline uint32_t GetBufferIndex() const { return m_FrameBufferIndex; }

	protected:
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_FrameBufferIndex;

	};
}
