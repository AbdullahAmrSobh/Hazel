#pragma once
#include "Hazel/RHI/RHIFrameBuffer.h"

namespace Hazel {

	class VulkanDevice;
	class VulkanRenderPass;
	struct VulkanFrameBufferDesc
	{
		VulkanRenderPass*			pRenderPass;
		uint32_t					Width; 
		uint32_t					Height;
		uint32_t					Layers;
		VkImage						ImageHandle;
		std::vector<VkImageView>	Views;
	};

	class VulkanFrameBuffer : public RHIFrameBuffer
	{
	public:
		VulkanFrameBuffer(const VulkanDevice* pDevice)
			: m_pDevice(pDevice), m_pRenderPass(nullptr), m_FrameBufferHandle(VK_NULL_HANDLE), m_ImageHandle(VK_NULL_HANDLE)
		{

		}

		VulkanFrameBuffer(const VulkanDevice* device, const VulkanFrameBufferDesc& desc);
		~VulkanFrameBuffer();

		void Init(const VulkanFrameBufferDesc& desc);

		inline auto GetHandle() const { return m_FrameBufferHandle; }
		inline auto GetImage() const { return m_ImageHandle; }
		inline auto& GetAttachmentViews() const { return m_AttachmentViews; }

		inline auto GetRenderPass() const { return m_pRenderPass; }

	private:
		const VulkanDevice*		 m_pDevice;
		VulkanRenderPass*		 m_pRenderPass;
		VkFramebuffer			 m_FrameBufferHandle;
		VkImage					 m_ImageHandle;
		std::vector<VkImageView> m_AttachmentViews;
	};
}
