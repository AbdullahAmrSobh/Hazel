#pragma once
#include "Hazel/RHI/RHICommandBuffer.h"

#include <vulkan/vulkan.h>

namespace Hazel {
	
	class VulkanDevice;
	class VulkanQueue;

	class VulkanCommandAllocator : public RHIObject
	{
	public:
		VulkanCommandAllocator(const VulkanDevice* pDevice, const VulkanQueue* pQueue);
		~VulkanCommandAllocator();

		inline VkCommandPool GetHandle() const { return m_CommandPoolHandle; }

		VkCommandBuffer AllocateCommandBuffer ();
		void FreeCommandBuffer(uint32_t count, VkCommandBuffer* cmd);

	private:
		const VulkanDevice* m_pDevice;
		VkCommandPool m_CommandPoolHandle;

	};

	class VulkanCommandBuffer : public RHICommandBuffer
	{
	public:
		VulkanCommandBuffer(VulkanCommandAllocator* pParantAllocator);
		~VulkanCommandBuffer();

		inline VkCommandBuffer GetHandle() const { return m_CommandBufferHandle; }
		
		inline const auto& GetWaitSemaphores() const { return m_WaitSemaphores; }
		inline void AddWaitSemaphore(VkSemaphore smeaphore) { m_WaitSemaphores.push_back(smeaphore); }

	public:
		virtual void Begin() final override;
		virtual void End() final override;

		virtual void BeginFrame(const RHIFrameBuffer* pTargetFrameBuffer, const RHIColor clearColor) final override;
		virtual void EndFrame() final override;

		virtual void BindGraphicsPipelineState(const RHIGraphicsPipelineState* pPipelineState) final override;

		virtual void BindDescriptorSets(const RHIPipelineLayout* pLayout, const RHIDescriptorSet* pSet) final override;

		virtual void Draw(const RHIVertexBuffer* pVertexBuffer) final override;
		virtual void Draw(const RHIVertexBuffer* pVertexBuffer, const RHIIndexBuffer* pIndexBuffer) final override;

		virtual void CopyResource(const RHIBufferCopyDesc& copyDesc) final override;

	private:
		VulkanCommandAllocator* m_pAllocator;
		VkCommandBuffer m_CommandBufferHandle;

		std::vector<VkSemaphore> m_WaitSemaphores;
	};

}
