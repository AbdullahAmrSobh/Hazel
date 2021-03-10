#pragma once
#include <vector>
#include <vulkan/vulkan.h>
namespace Hazel {

	class VulkanDevice;

	struct VulkanRenderPassDesc
	{
		std::vector<VkAttachmentDescription>	AttachmentDesc;
		std::vector<VkAttachmentReference>		AttachmentReferences;
		std::vector<VkSubpassDescription>		SubPassDesc;
		std::vector<VkSubpassDependency>		SubPassDependencies;
	};

	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(const VulkanDevice* pDevice)
			: m_pDevice(pDevice), m_RenderPassHandle(VK_NULL_HANDLE)
		{}

		VulkanRenderPass(const VulkanDevice* pDevice, const VulkanRenderPassDesc& desc);
		~VulkanRenderPass();

		void Init(const VulkanRenderPassDesc& desc);

		inline VkRenderPass GetHandle() const { return m_RenderPassHandle; }

	private:
		const VulkanDevice*				m_pDevice;
		VkRenderPass					m_RenderPassHandle;
	};
}
