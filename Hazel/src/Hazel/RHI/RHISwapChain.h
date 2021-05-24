#pragma once
#include "RHIObject.h"

namespace Hazel {

	class RHIFrameBuffer;
	class RHIFence;

	class RHISwapChain : public RHIObject
	{
	public:

		inline uint32_t GetCurrentFrameIndex() const { return m_CurrentFrameIndex; }
		inline uint32_t GetBackBuffersCount() const { return m_BackBuffersCount; }
		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		inline RHIFence* GetFence() { return m_pFence; };
		inline const RHIFence* GetFence() const { return m_pFence; };

		virtual void OnResize(uint32_t width, uint32_t height) = 0;
		virtual void Present() = 0;
		virtual void SwapBuffers() = 0;

		virtual RHIFrameBuffer** GetFrameBuffer() = 0;

	protected:
		uint32_t	m_CurrentFrameIndex;
		uint32_t	m_BackBuffersCount;
		RHIFence*	m_pFence;
		uint32_t	m_Width;
		uint32_t	m_Height;
	};
}
