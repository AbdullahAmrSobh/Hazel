#pragma once

#include "Hazel/RHI/RHIObject.h"
#include "Hazel/RHI/RHIFrameBuffer.h"
#include "Hazel/RHI/RHIPipelineState.h"
#include <array>

namespace Hazel {
	
	class RHIBuffer;
	class RHIStagingBuffer;
	class RHIUniformBuffer;
	class RHIVertexBuffer;
	class RHIIndexBuffer;

	class RHIDescriptorSet;

	struct RHIBufferCopyDesc
	{
		RHIBuffer*	SrcBuffer;
		size_t		SrcOffset;
		RHIBuffer*	DstBuffer;
		size_t		DstOffset;
		size_t		Size;
	};



	class RHICommandBuffer : public RHIObject
	{
	public:
		
		virtual void Begin() = 0;
		virtual void End() = 0;
		
		// Frame
		virtual void BeginFrame(const RHIFrameBuffer* pTargetFrameBuffer, const RHIColor clearColor = {0.5f, 0.7f, 0.9f, 1.0f}) = 0;
		virtual void EndFrame() = 0;
		
		// Pipeline Binding
		virtual void BindGraphicsPipelineState(const RHIGraphicsPipelineState* pPipelineState) = 0;
		
		// Resource Binding
		virtual void BindDescriptorSets(const RHIPipelineLayout* pLayout, const RHIDescriptorSet* pSet) = 0;

		// Drawing Comands
		virtual void Draw(const RHIVertexBuffer* pVertexBuffer) = 0;
		virtual void Draw(const RHIVertexBuffer* pVertexBuffer, const RHIIndexBuffer* pIndexBuffer) = 0;

		// Resource copying and moving
		virtual void CopyResource(const RHIBufferCopyDesc& copyDesc) = 0;
		// virtual void CopyResource(const RHIImage* src,  const RHIImage*  pDst, size_t offset, size_t size) = 0;
		// virtual void CopyResource(const RHIBuffer* src, const RHIImage*  pDst, size_t offset, size_t size) = 0;
		// virtual void CopyResource(const RHIImage* src,  const RHIBuffer* pDst, size_t offset, size_t size) = 0;

		// Barriers

		// virtual void CreateBarrier(const void* image) = 0;
		// virtual void CreateBarrier(const void* buffer) = 0;
		// virtual void CreateBarrier(const void* memory) = 0;

    };

}
