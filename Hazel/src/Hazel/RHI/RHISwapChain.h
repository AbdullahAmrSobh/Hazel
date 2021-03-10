#pragma once
#include "Hazel/RHI/RHIFrameBuffer.h"

namespace Hazel {

	class RHISwapChain
	{
	public:
		virtual ~RHISwapChain() = default;

		virtual void SwapFrameBuffers() = 0;
		virtual void Present() = 0;

		virtual uint32_t GetCurrentFrameBufferIndex() = 0;
		virtual uint32_t GetBackBuffersCount() = 0;
		virtual RHIFrameBuffer** GetFrameBuffers() = 0;
	};
}
