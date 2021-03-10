#pragma once
#include "Hazel/RHI/RHIShader.h"

#include "Hazel/RHI/RHISwapChain.h"
#include "Hazel/RHI/RHIFrameBuffer.h"
#include "Hazel/RHI/RHIShader.h"
#include "Hazel/RHI/RHIPipelineLayout.h"
#include "Hazel/RHI/RHIPipelineState.h"
#include "Hazel/RHI/RHICommandBuffer.h"

namespace Hazel {

	enum class RHIBackend
	{
		eNone,
		eVulkan,
	};

	class RHI
	{
	public:
		static RHI* CreateRHI(class Window* pWindow);
	
	public:
		
		RHI(Window* pWindow) : m_pWindow(pWindow) { }
		virtual ~RHI() = default;

		virtual Ref<RHIShader> CreateShader(const RHIPrecompiledShader& shader) const = 0;

		virtual Scope<RHIPipelineLayout> CreatePipelineLayout(const RHIPipelineLayoutDesc& layoutDesc) = 0;
		virtual Scope<RHIGraphicsPipelineState> CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& psoDesc) = 0;

		virtual Scope<RHICommandBuffer> AllocateCommandBuffer(uint32_t count) = 0;
		virtual void SubmitCommandBuffer(RHICommandBuffer* pCommandBuffer) = 0;

		virtual RHISwapChain* GetSwapChain() = 0;
	
	protected:
		Window* m_pWindow;
	
	};
}
