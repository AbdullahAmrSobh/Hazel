#pragma once
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanMemory.h"

namespace Hazel {

	class VulkanDeviceObject
	{
	protected:
		VulkanDeviceObject(const VulkanDevice* pDevice, VulkanMemoryAllocator* pAllocator)
			: m_pDevice(pDevice)
			, m_pAllocator(pAllocator)
		{ }

	protected:
		const VulkanDevice* m_pDevice;
		const VulkanMemoryAllocator* m_pAllocator;
	};

	static const VulkanDevice* GetVulkanDevice()
	{
		static const VulkanDevice* pDevice= nullptr;

		return pDevice;
	}

	static const VulkanMemoryAllocator* GetMemoryAllocator()
	{
		static const VulkanMemoryAllocator* pAllocator;
		return pAllocator;
	}

}
