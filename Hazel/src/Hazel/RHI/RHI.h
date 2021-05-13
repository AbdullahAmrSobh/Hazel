#pragma once
#include "Hazel/Debug/renderdoc_app.h"
#include "Hazel/Core/Window.h"

namespace Hazel 
{
	struct RHIPipelineLayoutDesc;
	struct RHIDescriptorSetLayoutDesc;
	struct RHIGraphicsPipelineStateDesc;

	class RHISwapChain;
	class RHIShaderCompiler;
	class RHIFence;
	class RHIDescriptorPool;
	class RHIPipelineLayout;
	class RHIGraphicsPipelineState;
	class RHICommandBuffer;
	class RHIUniformBuffer;
	class RHIStagingBuffer;
	class RHIVertexBuffer;
	class RHIIndexBuffer;

	enum class RHIBackend
	{
		eNone,
		eVulkan,
		eD3D12,
	};

	class RHI : private NonCopyable
	{
	public:
		static RHI* CreateRHI(class Window* pWindow);
	
		inline RHIBackend GetRHIBackend() const { return m_Backend; }
		inline Window* GetWindow() const { return m_pWindow; }

	public:
		
		RHI(Window* pWindow, const RHIBackend backend) 
			: m_pWindow(pWindow), m_Backend(backend)
		{ }

		virtual ~RHI() = default;

		virtual void OnInit() = 0;
		virtual void OnShutdown() = 0;
		virtual void OnUpdate() = 0;

		virtual RHISwapChain* GetSwapChain() = 0;
		virtual RHIShaderCompiler* GetShaderCompiler() = 0;

		virtual RHIFence* CreateFence() = 0;

		virtual RHIDescriptorPool* CreateDescriptorPool(const std::vector<RHIDescriptorSetLayoutDesc>& layouts, uint32_t maxSets) = 0;
		virtual RHIPipelineLayout* CreatePipelineLayout(const RHIPipelineLayoutDesc& desc) = 0;
		virtual RHIGraphicsPipelineState* CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& desc) = 0;

		virtual RHICommandBuffer* AllocateCommandBuffer() = 0;	
		virtual void ExecuteCommandBuffer(RHICommandBuffer* pCommandBuffer, RHIFence* pFence = nullptr) = 0;

		virtual RHIUniformBuffer* CreateUniformBuffer(size_t bufferSize) = 0;
		virtual RHIStagingBuffer* CreateStagingBuffer(size_t bufferSize) = 0;
		virtual RHIVertexBuffer* CreateVertexBuffer(uint32_t vertexCount, uint32_t stride) = 0;
		virtual RHIIndexBuffer* CreateIndexBuffer(uint32_t indciesCount, uint32_t stride) = 0;

	protected:
		Window* m_pWindow;
		RHIBackend m_Backend;

	};

	static inline RENDERDOC_API_1_1_2* s_pRenderdoc_api = nullptr;

}
