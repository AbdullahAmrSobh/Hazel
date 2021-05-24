#pragma once

#include "Hazel.h"

using namespace Hazel;

struct MvpUniform
{
	alignas(16) glm::mat4 Model;
	alignas(16) glm::mat4 View;
	alignas(16) glm::mat4 Proj;
};

struct MaterialUniform
{
	glm::vec3 Color;
};

class SandboxRHI : public Layer
{
public:
	SandboxRHI();
	virtual ~SandboxRHI() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& e) override;

private:
	RHI*									pRHI;
	RHISwapChain*							pSwapChain;
	// RHIFence*								pSwapFence;
	RHIShaderCompiler*						pShaderFactory;
	RHIFrameBuffer**						ppFrameBuffers;
	RHIPipelineLayout*						pPipelineLayout;
	RHIGraphicsPipelineState*				pGraphicsPipelineState;
	RHICommandBuffer**						ppCommandBuffers;
	RHIDescriptorPool*						pDescriptorsPool;
	RHIDescriptorSet*						pDescriptorSet;
	RHIStagingBuffer*						pStaginingBuffer;
	RHIVertexBuffer*						pVertexBuffer;
	RHIIndexBuffer*							pIndexBuffer;

	RHISampler*								pSampler;
	RHITexture2D*							pAmazingTexture;


	RHIUniformBuffer*						pMvpUB;
	MvpUniform								MvpData;
};
