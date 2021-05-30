#pragma once
#include "Hazel/RHI/RHI.h"

#include <vulkan/vulkan.h>

namespace Hazel 
{

	struct RHITexture2DDesc;
	struct  RHIDescriptorSetLayoutDesc;

	class VulkanPhysicalDeviceProperties;
	class VulkanDevice;
	class VulkanMemoryAllocator;
	class VulkanSwapChain;
	class VulkanShaderCompiler;
	class VulkanCommandAllocator;

	class VulkanDescriptorsLayoutManager;

	class VulkanBuffer;

	class VulkanRHI : public RHI
	{
	public:
		VulkanRHI(Window*);
		~VulkanRHI();

		virtual void OnInit() final override;
		virtual void OnShutdown() final override;
		virtual void OnUpdate() final override;

		// Get Handles

		virtual RHISwapChain* GetSwapChain() final override;
		virtual RHIShaderCompiler* GetShaderCompiler() final override;

		virtual RHIFence* CreateFence() final override;

		virtual RHIDescriptorPool* CreateDescriptorPool(const std::vector<RHIDescriptorSetLayoutDesc>& layouts, uint32_t maxSets) final override;
		virtual RHIPipelineLayout* CreatePipelineLayout(const RHIPipelineLayoutDesc& desc) final override;
		virtual RHIGraphicsPipelineState* CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& desc) final override;

		virtual RHICommandBuffer* AllocateCommandBuffer() final override;
		virtual void ExecuteCommandBuffer(RHICommandBuffer* pCommandBuffer, RHIFence* pFences) final override;

		virtual RHIUniformBuffer* CreateUniformBuffer(size_t bufferSize) final override;
		virtual RHIStagingBuffer* CreateStagingBuffer(size_t bufferSize) final override;
		virtual RHIVertexBuffer* CreateVertexBuffer(uint32_t vertexCount, uint32_t stride) final override;
		virtual RHIIndexBuffer* CreateIndexBuffer(uint32_t indciesCount, uint32_t stride) final override;

		virtual RHISampler* CreateSampler(const RHISamplerDesc& desc) final override;

		virtual RHITexture2D* CreateTexture2D(const RHITexture2DDesc& desc) final override;
		 
	private:
		void DestroyDebugUtilsMessenger();
	
		VkPhysicalDevice SelectPhysicalDevice(const std::vector<VkPhysicalDevice>& physicalDevices) const;

	private:
		VkInstance							m_InstanceHandle;
		VkDebugUtilsMessengerEXT			m_DebugUtilsMessengerHandle;
		VkSurfaceKHR						m_SurfaceHandle;
		VulkanDevice*						m_pDevice;
		VulkanMemoryAllocator*				m_pAllocator;

		VulkanSwapChain*					m_pSwapChain;
		VulkanShaderCompiler*				m_pShaderCompiler;

		VulkanCommandAllocator*				m_pCommandAllocator;
		VulkanDescriptorsLayoutManager*		m_pDescriptorsLayoutManager;

	};
}
