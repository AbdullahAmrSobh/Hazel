#include "SandboxRHI.h"
#include <filesystem> 
#include <fstream>

#include <glm/glm.hpp>
#include <glm/glm/ext/matrix_transform.hpp>
#include <glm/glm/ext/matrix_clip_space.inl>

std::string ReadFile(const std::string& filepath)
{
	HZ_PROFILE_FUNCTION();

	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
	if (in)
	{
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		if (size != -1)
		{
			result.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&result[0], size);
		}
		else
		{
			HZ_ERROR("Could not read from file '{0}'", filepath);
		}
	}
	else
	{
		HZ_ERROR("Could not open file '{0}'", filepath);
	}

	return result;
}

SandboxRHI::SandboxRHI()
{
	HZ_INFO("SandboxRHI Application layer is initalized");
	
	pRHI			= Application::GetRHI();
	pSwapChain		= pRHI->GetSwapChain();
	ppFrameBuffers	= pSwapChain->GetFrameBuffer();
	pShaderFactory	= pRHI->GetShaderCompiler();
}

void SandboxRHI::OnAttach()
{
	HZ_INFO("SandboxRHI Application layer is attached");

	pRHI->OnInit();

	// Define the descriptorset layout of main uniform buffers
	RHIDescriptorSetLayoutDesc dsl{ "u_SceneProj", { 
		{ "u_Model", 0, RHIDescriptorType::eUniformBuffer, 1, RHIPipelineShaderStage::eVertexShader },
		{ "u_ViewProj", 1, RHIDescriptorType::eUniformBuffer, 1, RHIPipelineShaderStage::eVertexShader }
	} };

	// Create descriptor allocator and descriptor set
	{
		std::vector<RHIDescriptorSetLayoutDesc> descriptorSetLayouts{ dsl };
		pDescriptorsPool = pRHI->CreateDescriptorPool(descriptorSetLayouts, 2);
		pDescriptorSet = pDescriptorsPool->AllocateDescriptorSet(dsl);
	}

	// Creates the pipeline layout
	{
		RHIPipelineLayoutDesc desc	= {};
		desc.DescriptorsLayouts.push_back(dsl);

		pPipelineLayout = pRHI->CreatePipelineLayout(desc);
	}

	// Graphics Pipeline Creation
	{
		Hazel::File pVertexShaderBin	= ReadBinaryFile("./assets/shaders/vert.spv");
		Hazel::File pPixelShaderBin		= ReadBinaryFile("./assets/shaders/frag.spv");

		RHIShader* pVertexShader = pShaderFactory->CompileShader(pVertexShaderBin);
		RHIShader* pPixelShader  = pShaderFactory->CompileShader(pPixelShaderBin);

		HZ_ASSERT(pVertexShader != nullptr && pPixelShader != nullptr);

		RHIVertexBufferLayout vboLayout({
				{ "Position", RHIFormat::eFloat2 },
				{ "Color",	  RHIFormat::eFloat3 }
			});

		RHIGraphicsPipelineStateDesc stateDesc	= {};
		stateDesc.pVS							= pVertexShader;
		stateDesc.pPS							= pPixelShader;
		stateDesc.pLayout						= pPipelineLayout;
		stateDesc.pVertexLayout					= &vboLayout;
		
		pGraphicsPipelineState = pRHI->CreateGraphicsPipelineState(stateDesc);
	}

	// Create Vertex Buffer, and Index Buffer
	{
		float vertcies[] = { 
			-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,	0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,	0.5f, 1.0f, 1.0f, 1.0f 
		};

		uint32_t indcies[] = {0, 1, 2, 2, 3, 0};

		pVertexBuffer	 = pRHI->CreateVertexBuffer(4, 5 * sizeof(float));
		pIndexBuffer	 = pRHI->CreateIndexBuffer(6, sizeof(uint32_t));
		pStaginingBuffer = pRHI->CreateStagingBuffer(sizeof(vertcies) + sizeof(indcies));
		
		pStaginingBuffer->CopyData(sizeof(vertcies), vertcies, 0);
		pStaginingBuffer->CopyData(sizeof(indcies), indcies, sizeof(indcies));

		RHIFence* pResourceCopyFence = pRHI->CreateFence();
		RHICommandBuffer* pResourceCopyCmdBuffer1 = pRHI->AllocateCommandBuffer();
		RHICommandBuffer* pResourceCopyCmdBuffer2 = pRHI->AllocateCommandBuffer();

		RHIBufferCopyDesc copyDesc  = {};
		pResourceCopyCmdBuffer1->Begin();
		copyDesc.SrcBuffer			= pStaginingBuffer->GetBuffer();
		copyDesc.DstBuffer			= pVertexBuffer->GetBuffer();
		copyDesc.SrcOffset			= 0;
		copyDesc.DstOffset			= 0;
		copyDesc.Size				= pVertexBuffer->GetBuffer()->GetSize();
		pResourceCopyCmdBuffer1->CopyResource(copyDesc);
		pResourceCopyCmdBuffer1->End();

		pResourceCopyCmdBuffer2->Begin();
		copyDesc.SrcBuffer	= pStaginingBuffer->GetBuffer();
		copyDesc.DstBuffer	= pIndexBuffer->GetBuffer();
		copyDesc.SrcOffset	= 0;
		copyDesc.DstOffset	= 0;
		copyDesc.Size		= pIndexBuffer->GetBuffer()->GetSize();
		pResourceCopyCmdBuffer2->CopyResource(copyDesc);
		pResourceCopyCmdBuffer2->End();

		pRHI->ExecuteCommandBuffer(pResourceCopyCmdBuffer1, pResourceCopyFence);
		pResourceCopyFence->Wait();
		
		pResourceCopyFence->Reset();
		
		pRHI->ExecuteCommandBuffer(pResourceCopyCmdBuffer2, pResourceCopyFence);
		pResourceCopyFence->Wait();


		delete pResourceCopyFence;
		delete pResourceCopyCmdBuffer1, pResourceCopyCmdBuffer2;
	}

	// Create uniform buffer object, and binds a memory to that buffer
	{
		pMvpUB = pRHI->CreateUniformBuffer(sizeof(MvpUniform));

		MvpUniform ubo{};
		ubo.Model = glm::mat4x4(1);
		ubo.View = glm::mat4x4(1);
		ubo.Proj = glm::mat4x4(1);
		// ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// ubo.Proj = glm::perspective(glm::radians(45.0f), (float)1600 / (float)720, 0.1f, 10.0f);

		pMvpUB->SetData(sizeof(ubo), &ubo);
		// pMvpUB->Bind(&pMvpData);
		// pMaterialUB->Bind(&pMaterialData);
	}

	// Bind a uniform buffer with a descriptor
	{
		pDescriptorSet->BindUiformBuffer(0, pMvpUB);
	}

	// Records command buffers
	uint32_t back_buffers_count = pSwapChain->GetBackBuffersCount();

	ppCommandBuffers = reinterpret_cast<RHICommandBuffer**>(malloc(back_buffers_count));
	
	for (int i = 0; i < back_buffers_count; i++)
	{
		ppCommandBuffers[i] = pRHI->AllocateCommandBuffer();

		RHICommandBuffer* pCmdBuffer = ppCommandBuffers[i];
		
		pCmdBuffer->Begin();
		pCmdBuffer->BeginFrame(ppFrameBuffers[i], { 1.0f, 1.0f, 1.0f, 1.0f});
		pCmdBuffer->BindGraphicsPipelineState(pGraphicsPipelineState);
		pCmdBuffer->BindDescriptorSets(pPipelineLayout, pDescriptorSet);
		pCmdBuffer->Draw(pVertexBuffer, pIndexBuffer);
		pCmdBuffer->EndFrame();
		pCmdBuffer->End();
	}

}

void SandboxRHI::OnDetach()
{
	HZ_INFO("SandboxRHI Application layer is Dettached, and destroyed");
	pRHI->OnShutdown();
}

void SandboxRHI::OnUpdate(Hazel::Timestep ts)
{
	// s_pRenderdoc_api->StartFrameCapture(NULL, NULL);
	uint32_t currentFrameIndex = pSwapChain->GetCurrentFrameIndex();
	pRHI->ExecuteCommandBuffer(ppCommandBuffers[currentFrameIndex]);
	pRHI->OnUpdate();
	// s_pRenderdoc_api->EndFrameCapture(NULL, NULL);
}

void SandboxRHI::OnImGuiRender()
{
	HZ_ERROR("ImGui is not yet implemented OnImGuiRender");
}

void SandboxRHI::OnEvent(Hazel::Event& e)
{
	HZ_CRITICAL("An Event is unhandled {0}", e.ToString());
}
