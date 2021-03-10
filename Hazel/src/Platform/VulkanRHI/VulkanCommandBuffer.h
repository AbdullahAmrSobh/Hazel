#pragma once
#include <vulkan/vulkan.h>

#include "Hazel/RHI/RHICommandBuffer.h"


#include "Platform/VulkanRHI/VulkanCommon.h"
#include "Platform/VulkanRHI/VulkanDevice.h"

#include "Platform/VulkanRHI/VulkanRenderPass.h"
#include "Platform/VulkanRHI/VulkanFrameBuffer.h"

#include "Platform/VulkanRHI/VulkanPipelineLayout.h"
#include "Platform/VulkanRHI/VulkanGraphicsPipelineState.h"

namespace Hazel {

	class VulkanDevice;

	class VulkanCommandsAllocator
	{
	public:
		VulkanCommandsAllocator(const VulkanDevice* pDevice, uint32_t queueFamilyIndex);
		~VulkanCommandsAllocator();

		std::vector<VulkanCommandBuffer*> AllocateCommandBuffers(uint32_t count, VkCommandBufferLevel level);

		void FreeCommandBuffers(std::vector<VkCommandBuffer> commandBuffers);

	private:
		const VulkanDevice* m_pDevice;
		VkCommandPool m_CommandPooLHandle;

	};

	class VulkanCommandBuffer : public RHICommandBuffer
	{
	public:
		VulkanCommandBuffer(const VulkanDevice* pDevice, VkCommandBuffer bufferHandle);

		void RecordVulkanDebugCommands(VulkanRenderPass* pRenderPass, VulkanFrameBuffer* pTargetFrameBuffer, VulkanGraphicsPipelineState* pPso);
		virtual void RecordDebugCommands(RHIFrameBuffer* pRenderTarget, RHIGraphicsPipelineState* pPso) override;

		inline VkCommandBuffer GetHandle() const { return m_CommandBufferHandle; }
	private:
		const VulkanDevice*					m_pDevice;
		VkCommandBuffer						m_CommandBufferHandle;
	};
}
