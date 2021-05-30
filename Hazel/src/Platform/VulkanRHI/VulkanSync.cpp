#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"

#include "Platform/VulkanRHI/VulkanResources.h"
#include "Platform/VulkanRHI/VulkanRHI.h"

namespace Hazel {

	RHIFence* VulkanRHI::CreateFence()
	{
		return new VulkanFence(m_pDevice);
	}


	VulkanSemaphore::VulkanSemaphore(const VulkanDevice* pDevice)
		: m_pDevice(pDevice)
		, m_SemaphoreHandle(VK_NULL_HANDLE)
	{
		VkSemaphoreCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;

		VK_CHECK_RESULT(vkCreateSemaphore(m_pDevice->GetHandle(), &createInfo, nullptr, &m_SemaphoreHandle),
			"Failed to create  a semaphore");
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		vkDestroySemaphore(m_pDevice->GetHandle(), m_SemaphoreHandle, nullptr);
	}

	VulkanFence::VulkanFence(const VulkanDevice* pDevice)
		: m_pDevice(pDevice)
	{
		VkFenceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;

		VK_CHECK_RESULT(vkCreateFence(m_pDevice->GetHandle(), &createInfo, nullptr, &m_FenceHandle)
			, "Failed to create a fence");
	}

	VulkanFence::~VulkanFence()
	{
		vkDestroyFence(m_pDevice->GetHandle(), m_FenceHandle, nullptr);
	}

	void VulkanFence::Reset()
	{
		VK_CHECK_RESULT(vkResetFences(m_pDevice->GetHandle(), 1, &m_FenceHandle), "Failed to reset a fence");
	}

	void VulkanFence::Wait()
	{
		VK_CHECK_RESULT(vkWaitForFences(m_pDevice->GetHandle(), 1, &m_FenceHandle, VK_TRUE, UINT64_MAX), "Failed to wait for fence");
	}

	bool VulkanFence::GetFenceStatus()
	{
		VkResult result = vkGetFenceStatus(m_pDevice->GetHandle(), m_FenceHandle);
		if (result == VK_SUCCESS)
			return true;
		if (result == VK_NOT_READY)
			return false;

		VK_CHECK_RESULT(result, "Failed to check for fence status");
		return false;
	}

}
