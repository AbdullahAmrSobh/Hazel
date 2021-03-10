#pragma once
#include "Hazel/RHI/RHIFrameBuffer.h"
#include "Hazel/RHI/RHIPipelineState.h"
#include "Hazel/RHI/RHIPipelineLayout.h"

namespace Hazel {
	
	class RHICommandBuffer
	{
	public:
		virtual ~RHICommandBuffer() = default;
		virtual void RecordDebugCommands(RHIFrameBuffer* pRenderTarget, RHIGraphicsPipelineState* pPso) {};
	};
}
