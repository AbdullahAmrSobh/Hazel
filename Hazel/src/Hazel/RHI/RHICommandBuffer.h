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

	struct RHITextureCopyDesc
	{
		RHITexture* pSrcTexture;
		RHITexture* pDstTexture;
		uint32_t Width;
		uint32_t Height;
	};


	struct RHITextureToBufferCopyDesc
	{

	};

	struct RHIBufferToTextureCopyDesc
	{
		RHITexture*		pDstTexture;
		RHIBuffer*		pSrcBuffer;
		size_t			SrcOffset;
		size_t			Size;
		uint32_t		Width;
		uint32_t		Height;
	};


	class RHICommandBuffer : public RHIObject
	{
	public:
		
		virtual void Begin() = 0;
		virtual void End() = 0;
		
		// Frame
		virtual void BeginFrame(const RHIFrameBuffer* pTargetFrameBuffer, const RHIColor clearColor = {1.0f, 1.0f, 1.0f, 1.0f}) = 0;
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
		virtual void CopyResource(const RHITextureCopyDesc& copyDesc) = 0;
		virtual void CopyResource(const RHITextureToBufferCopyDesc& copyDesc) = 0;
		virtual void CopyResource(const RHIBufferToTextureCopyDesc& copyDesc) = 0;

		// Barriers
		virtual void PipelineBarrier() = 0;
		// virtual void CreateBarrier(const void* image) = 0;
		// virtual void CreateBarrier(const void* buffer) = 0;
		// virtual void CreateBarrier(const void* memory) = 0;

    };

	class RHIGraphicsCommandBuffer;		// Binds a pipeline, and renders 
	class RHIComputeCommandBuffer;		// Binds a pipeline,
	class RHIResourceCommandBuffer;		// Move/Copy resources,

	class RHICommandList
	{
	public:
		std::vector<RHICommandBuffer*> pCommandBuffers;
		class RHIFence* pFence;
	};

}
