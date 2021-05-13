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
	RHIShaderCompiler*						pShaderFactory;
	RHIFrameBuffer**						ppFrameBuffers;
	RHIPipelineLayout*						pPipelineLayout;
	RHIGraphicsPipelineState*				pGraphicsPipelineState;
	RHICommandBuffer**						ppCommandBuffers;
	RHIDescriptorPool*						pDescriptorsPool;
	RHIDescriptorSet*						pDescriptorSet;

	// Buffers and resources
	RHIStagingBuffer*						pStaginingBuffer;
	RHIVertexBuffer*						pVertexBuffer;
	RHIIndexBuffer*							pIndexBuffer;
	RHIUniformBuffer*						pMvpUB;
	// RHIUniformBuffer*						pMaterialUB;
	// Uniform data
	MvpUniform								MvpData;
	// MaterialUniform*						pMaterialData;
};
