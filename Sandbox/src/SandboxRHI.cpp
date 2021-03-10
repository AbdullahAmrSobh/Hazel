#include "SandboxRHI.h"


SandboxRHI::SandboxRHI()
{
	HZ_INFO("SandboxRHI Application layer is initalized");
	pRHI = Hazel::Application::GetRHI();

}

void SandboxRHI::OnAttach()
{
	HZ_INFO("SandboxRHI Application layer is attached");

	// Create a pipeline layout
	{
		Hazel::RHIPipelineLayoutDesc layoutDesc = {};
		pPipelineLayout = pRHI->CreatePipelineLayout(layoutDesc);
	}

	// Create a pipeline object
	{
		Hazel::RHIGraphicsPipelineStateDesc psoDesc = {};

		Hazel::RHIVertexInputStateDesc vbInputState = {};
		psoDesc.pVertexInputStateDesc = &vbInputState;
		
		Hazel::RHIViewportState viewportState	= {};
		viewportState.Viewport.Width			= 1280;
		viewportState.Viewport.Height			= 720;
		viewportState.Viewport.OffsetX			= 0;
		viewportState.Viewport.OffsetY			= 0;
		viewportState.Viewport.Min				= 0.0f;
		viewportState.Viewport.Max				= 1.0f;
		viewportState.Scissor.Width				= 1280;
		viewportState.Scissor.Width				= 1280;
		viewportState.Scissor.Height			= 720;
		viewportState.Scissor.OffsetX			= 0;
		viewportState.Scissor.OffsetY			= 0;
		psoDesc.pViewportStateDesc = &viewportState;
		
		psoDesc.pPipelineLayout = pPipelineLayout.get();
		
		auto vsFile = Hazel::ReadBinaryFile("./assets/shaders/frag.spv");
		auto psFile = Hazel::ReadBinaryFile("./assets/shaders/vert.spv");

		Hazel::RHIPrecompiledShader precompiledVertexShader(vsFile.size(), vsFile.data());
		Hazel::RHIPrecompiledShader precompiledPixelShader(psFile.size(), psFile.data());

		auto vertexShader = pRHI->CreateShader(precompiledVertexShader);
		auto pixelShader = pRHI->CreateShader(precompiledPixelShader);

		psoDesc.pPixelShader = vertexShader;
		psoDesc.pVertexShader = pixelShader;


		pPipelineObject = pRHI->CreateGraphicsPipelineState(psoDesc);
	}
	
	// Obtain FrameBuffers
	{
		pFrameBuffers.resize(pRHI->GetSwapChain()->GetBackBuffersCount());
		memcpy(pFrameBuffers.data(), pRHI->GetSwapChain()->GetFrameBuffers(), pRHI->GetSwapChain()->GetBackBuffersCount());
	}

	// Allocate CommandBuffers
	{
		pCommandBuffers = pRHI->AllocateCommandBuffer(pRHI->GetSwapChain()->GetBackBuffersCount());
	}
	
	// Record Commands
	{
		for (int i = 0; i < pCommandBuffers.size(); i++)
			pCommandBuffers[i]->RecordDebugCommands(pFrameBuffers[i], pPipelineObject.get());
	}

}

void SandboxRHI::OnDetach()
{
	HZ_INFO("SandboxRHI Application layer is Dettached, and destroyed");
}

void SandboxRHI::OnUpdate(Hazel::Timestep ts)
{
	pRHI->SubmitCommandBuffer(pCommandBuffers[pRHI->GetSwapChain()->GetCurrentFrameBufferIndex()].get());
}

void SandboxRHI::OnImGuiRender()
{
	HZ_ERROR("ImGui is not yet implemented");
}

void SandboxRHI::OnEvent(Hazel::Event& e)
{
	HZ_CRITICAL("An Event is unhandled {0}", e.ToString());
}
