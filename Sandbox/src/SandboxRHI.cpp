#include "SandboxRHI.h"
#include <filesystem> 
#include <fstream>

#include <glm/glm.hpp>
#include <glm/glm/ext/matrix_transform.hpp>
#include <glm/glm/ext/matrix_clip_space.inl>

#include <stb_image/stb_image.h>

#include <imgui/imgui.h>

static std::string ReadFile(const std::string& filepath)
{
	HZ_PROFILE_FUNCTION();

	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
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
	
	in.close();

	return result;
}

struct Image
{
	int32_t Width;
	int32_t Height;
	int32_t Channels;
	stbi_uc* Pixels;
	size_t Size;
};

static Image ReadImage(const std::string& path)
{
	Image img;
	img.Pixels = stbi_load(path.c_str(), &img.Width, &img.Height, &img.Channels, STBI_rgb_alpha);
	img.Size = img.Width * img.Height;
	HZ_CORE_ASSERT(img.Pixels, "Failed to open asset");
	return img;
}

SandboxRHI::SandboxRHI()
{
	HZ_INFO("SandboxRHI Application layer is initalized");
	
	pRHI			= Application::GetRHI();
	pSwapChain		= pRHI->GetSwapChain();
	pShaderFactory	= pRHI->GetShaderCompiler();
}

void SandboxRHI::OnAttach()
{
	HZ_INFO("SandboxRHI Application layer is attached");

	pRHI->OnInit();

	// Define the descriptorset layout of main uniform buffers
	RHIDescriptorSetLayoutDesc dsl{ "u_SceneProj", { 
		{ "u_Mvp", 0, RHIDescriptorType::eUniformBuffer, 1, RHIPipelineShaderStage::eVertexShader },
		{ "u_Texture", 1, RHIDescriptorType::eTexture, 1, RHIPipelineShaderStage::ePixelShader }
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
		std::string vertexShaderSource = ReadFile("assets/shaders/vertexShader.vert");
		std::string pixelShaderSource = ReadFile("assets/shaders/fragmentShader.frag");

		RHIShader* pVertexShader = pShaderFactory->CompileFromSource(vertexShaderSource, RHIShaderType::eVertex, std::string("vertexShader"));
		RHIShader* pPixelShader  = pShaderFactory->CompileFromSource(pixelShaderSource, RHIShaderType::ePixel, std::string("pixelShader"));
		HZ_ASSERT(pVertexShader != nullptr && pPixelShader != nullptr);

		RHIVertexBufferLayout vboLayout({
				{ "Position", RHIFormat::eFloatRGB },
				{ "TexCoors", RHIFormat::eFloatRG },
				{ "Color",	  RHIFormat::eFloatRGB }
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
		struct Vertex
		{
			float position[3];
			float TexCoords[2];
			float VertexColor[3];
		};

		std::array<Vertex, 4> vertexData = {
			// Position, Tex Coords, Color 
			Vertex({{ -0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}),
			Vertex({{  0.5f, -0.5f, 0.0f}, { 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}),
			Vertex({{  0.5f,  0.5f, 0.0f}, { 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}),
			Vertex({{ -0.5f,  0.5f, 0.0f}, { 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}})
		};

		std::array<uint32_t, 6> indexData = {0, 1, 2, 2, 3, 0};

		Image textureData = ReadImage("./assets/textures/texture.jpg");

		size_t vertexDataSize		= vertexData.size() * sizeof(Vertex);
		size_t indexDataSize		= indexData.size() * sizeof(uint32_t);
		size_t amazingTextureSize	= static_cast<size_t>(textureData.Width) * textureData.Height * 4;

		pStaginingBuffer	= pRHI->CreateStagingBuffer(vertexDataSize + indexDataSize + amazingTextureSize);

		pVertexBuffer		= pRHI->CreateVertexBuffer(vertexData.size(), sizeof(Vertex));
		pIndexBuffer		= pRHI->CreateIndexBuffer(indexData.size(), sizeof(uint32_t));
		
		RHISamplerDesc samplerDesc	= {};
		samplerDesc.Filter			= RHIFilter::eNearest;
		samplerDesc.AddressU		= RHITextureAddressMode::eRepeat;
		samplerDesc.AddressV		= RHITextureAddressMode::eRepeat;
		samplerDesc.AddressW		= RHITextureAddressMode::eRepeat;
		samplerDesc.MipLODBias		= 1;
		samplerDesc.MaxAnisotropy	= 1.0;
		samplerDesc.ComparisonFunc	= RHIComparsionOperation::eAlways;
		samplerDesc.BorderColor[4]	= {};
		samplerDesc.MinLOD			= 0.0f;
		samplerDesc.MaxLOD			= 0.0f;
		samplerDesc.Border			= RHISamplerBorderColor::eIntOpaqueBlack;
		pSampler					= pRHI->CreateSampler(samplerDesc);

		RHITexture2DDesc textureDesc	= {};
		textureDesc.Width				= textureData.Width;
		textureDesc.Height				= textureData.Height;

		switch (textureData.Channels)
		{
		case 1:  textureDesc.Format = RHIFormat::eSRGB_R8; break;
		case 2:  textureDesc.Format = RHIFormat::eSRGB_RG8; break;
		case 3:  textureDesc.Format = RHIFormat::eSRGB_RGB8; break;
		case 4:  textureDesc.Format = RHIFormat::eSRGB_RGBA8; break;
		default: HZ_ASSERT(false, "Something went wrong!");
		}

		textureDesc.Format = RHIFormat::eSRGB_RGBA8;

		textureDesc.MipLevels			= 1;
		textureDesc.SampleCount			= 1;

		pAmazingTexture = pRHI->CreateTexture2D(textureDesc);

		{
			void* target = pStaginingBuffer->Lock(0, vertexDataSize);
			memcpy(target, vertexData.data(), vertexDataSize);
			pStaginingBuffer->Unlock();
		}

		{
			void* target = pStaginingBuffer->Lock(0 + vertexDataSize, indexDataSize);
			memcpy(target, indexData.data(), indexDataSize);
			pStaginingBuffer->Unlock();
		}

		{
			void* target = pStaginingBuffer->Lock(0 + vertexDataSize + indexDataSize, amazingTextureSize);
			memcpy(target, textureData.Pixels, amazingTextureSize);
			pStaginingBuffer->Unlock();
		}

		// A fence to prevent rendering before all staged resources are copied.
		RHIFence* pResourceCopyFence = pRHI->CreateFence();
		RHICommandBuffer* pResourceCopyCmdBuffer = pRHI->AllocateCommandBuffer();

		pResourceCopyCmdBuffer->Begin();

		// Copy Vertex buffer data
		{
			RHIBufferCopyDesc copyDesc	= {};
			copyDesc.SrcBuffer			= pStaginingBuffer->GetBuffer();
			copyDesc.DstBuffer			= pVertexBuffer->GetBuffer();
			copyDesc.SrcOffset			= 0;
			copyDesc.DstOffset			= 0;
			copyDesc.Size				= vertexDataSize;
			pResourceCopyCmdBuffer->CopyResource(copyDesc);
		}

		// Copy Index Buffer data
		{
			RHIBufferCopyDesc copyDesc = {};
			copyDesc.SrcBuffer = pStaginingBuffer->GetBuffer();
			copyDesc.DstBuffer = pIndexBuffer->GetBuffer();
			copyDesc.SrcOffset = vertexDataSize;
			copyDesc.DstOffset = 0;
			copyDesc.Size = indexDataSize;
			pResourceCopyCmdBuffer->CopyResource(copyDesc);
		}

		// Copy Texture's Image data
		{
			RHIBufferToTextureCopyDesc copyDesc = {};
			copyDesc.pSrcBuffer		= pStaginingBuffer->GetBuffer();
			copyDesc.pDstTexture	= pAmazingTexture->GetTextureResource();
			copyDesc.SrcOffset		= indexDataSize + vertexDataSize;
			copyDesc.Size			= amazingTextureSize;
			copyDesc.Width			= pAmazingTexture->GetWidth();
			copyDesc.Height			= pAmazingTexture->GetHeight();

			pResourceCopyCmdBuffer->CopyResource(copyDesc);
		}

		pResourceCopyCmdBuffer->End();

		pRHI->ExecuteCommandBuffer(pResourceCopyCmdBuffer, pResourceCopyFence);
		
		pResourceCopyFence->Wait();

		// delete pResourceCopyFence;
		delete pResourceCopyCmdBuffer;
	}

	// Create uniform buffer object, and binds a memory to that buffer
	{
 		pMvpUB = pRHI->CreateUniformBuffer(sizeof(MvpUniform));

		MvpData.Model = glm::mat4x4(1);
		MvpData.View = glm::lookAt(glm::vec3(2.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		MvpData.Proj = glm::perspective(glm::radians(45.0f), (float)1600 / (float)720, 0.1f, 10.0f);

		pMvpUB->SetData(sizeof(MvpUniform), &MvpData);
	}

	// Bind a uniform buffer with a descriptor
	{
		pDescriptorSet->BindUiformBuffer(0, pMvpUB);
		pDescriptorSet->BindTexture(1, pAmazingTexture, pSampler);
	}

	// Records command buffers
	uint32_t back_buffers_count = pSwapChain->GetBackBuffersCount();

	ppCommandBuffers = reinterpret_cast<RHICommandBuffer**>(malloc(back_buffers_count));
	
	for (uint32_t i = 0; i < back_buffers_count; i++)
	{
		ppCommandBuffers[i] = pRHI->AllocateCommandBuffer();
		pCommandBufferSyncFences.push_back(pRHI->CreateFence());
	}

}

void SandboxRHI::OnDetach()
{
	HZ_INFO("SandboxRHI Application layer is dettached, and destroyed");
	pRHI->OnShutdown();
}

void SandboxRHI::OnUpdate(Hazel::Timestep ts)
{
	RHIFrameBuffer* pCurrentFrameBUffer = pSwapChain->AcquireNextFrame();

	uint32_t currentFrameIndex = pSwapChain->GetCurrentFrameIndex();

	ppCommandBuffers[currentFrameIndex]->Reset();
	ppCommandBuffers[currentFrameIndex]->Begin();
	ppCommandBuffers[currentFrameIndex]->BeginFrame(pCurrentFrameBUffer, { 0.3f, 0.6f, 0.9f, 1.0f });
	ppCommandBuffers[currentFrameIndex]->BindGraphicsPipelineState(pGraphicsPipelineState);
	ppCommandBuffers[currentFrameIndex]->BindDescriptorSets(pPipelineLayout, pDescriptorSet);
	ppCommandBuffers[currentFrameIndex]->Draw(pVertexBuffer, pIndexBuffer);
	ppCommandBuffers[currentFrameIndex]->EndFrame();
	ppCommandBuffers[currentFrameIndex]->End();

	// pCommandBufferSyncFences[currentFrameIndex]->Wait();
	// pCommandBufferSyncFences[currentFrameIndex]->Reset();
	pRHI->ExecuteCommandBuffer(ppCommandBuffers[currentFrameIndex]);
	pSwapChain->Present();

	pRHI->OnUpdate();

	static auto startTime = std::chrono::high_resolution_clock::now();

	glm::vec3 eye;
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	MvpData.Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	pMvpUB->SetData(sizeof(MvpData), &MvpData);

}

void SandboxRHI::OnImGuiRender()
{
	// ImGui::Begin("Hello, World!");
	// ImGui::Text("I'm ImGui :)");
	// ImGui::End();
}

void SandboxRHI::OnEvent(Hazel::Event& e)
{
	// HZ_CRITICAL("An Event is unhandled {0}", e.ToString());
}
