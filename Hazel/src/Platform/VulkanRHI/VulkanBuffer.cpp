#include "hzpch.h"
#include "Platform/VulkanRHI/VulkanUtils.h"
#include "Platform/VulkanRHI/VulkanDevice.h"
#include "Platform/VulkanRHI/VulkanMemory.h"
#include "Platform/VulkanRHI/VulkanResources.h"
#include "Platform/VulkanRHI/VulkanRHI.h"

namespace Hazel {

	RHIUniformBuffer* VulkanRHI::CreateUniformBuffer(size_t bufferSize)
	{
		return new VulkanUniformBuffer(m_pDevice, m_pAllocator, bufferSize);
	}

	RHIStagingBuffer* VulkanRHI::CreateStagingBuffer(size_t bufferSize)
	{
		return new VulkanStagingBuffer(m_pDevice, m_pAllocator, bufferSize);
	}

	RHIVertexBuffer* VulkanRHI::CreateVertexBuffer(uint32_t vertexCount, uint32_t stride)
	{
		return new VulkanVertexBuffer(m_pDevice, m_pAllocator, vertexCount, stride);
	}

	RHIIndexBuffer* VulkanRHI::CreateIndexBuffer(uint32_t indciesCount, uint32_t stride)
	{
		return new VulkanIndexBuffer(m_pDevice, m_pAllocator, indciesCount, stride);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	VulkanBuffer::VulkanBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator)
		: m_pDevice(pDevice)
		, m_pAllocator(pAllocator)
		, m_BufferHandle(VK_NULL_HANDLE)
		, m_Allocation(VK_NULL_HANDLE)
		, m_AllocationInfo({})
	{
		
	}

	VulkanBuffer::VulkanBuffer(
		const VulkanDevice * pDevice, 
		const VulkanMemoryAllocator * pAllocator, 
		const VkBufferCreateInfo & createInfo,
		const VmaAllocationCreateInfo & allocInfo
	)
		: m_pDevice(pDevice)
		, m_pAllocator(pAllocator)
		, m_BufferHandle(VK_NULL_HANDLE)
		, m_Allocation(VK_NULL_HANDLE)
	{
		m_BufferSize = SIZE_MAX;
		Init(createInfo, allocInfo);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		vmaDestroyBuffer(m_pAllocator->GetHandle(), m_BufferHandle, m_Allocation);
	}

	void VulkanBuffer::Init(const VkBufferCreateInfo& createInfo, const VmaAllocationCreateInfo& allocationInfo)
	{
		VK_CHECK_RESULT(vmaCreateBuffer(m_pAllocator->GetHandle(), &createInfo, &allocationInfo, &m_BufferHandle, &m_Allocation, &m_AllocationInfo)
			, "Failed to create buffer, and or allocate memory for it");
		m_BufferSize = createInfo.size;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	VulkanStagingBuffer::VulkanStagingBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, size_t size)
		: m_pDevice(pDevice)
		, m_pAllocator(pAllocator)
	{
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.size = size;
		createInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount;
		createInfo.pQueueFamilyIndices;

		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.flags = 0;
		allocationInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;


		m_pBuffer = new VulkanBuffer(m_pDevice, m_pAllocator, createInfo, allocationInfo);
	}

	VulkanStagingBuffer::~VulkanStagingBuffer()
	{
		delete m_pBuffer;
	}

	void* VulkanStagingBuffer::Lock(size_t offset, size_t size)
	{
		void* pTarget;
		// VK_CHECK_RESULT(vmaMapMemory(m_pAllocator->GetHandle(), AsVulkanBuffer(m_pBuffer)->GetAllocation(), &pTarget), "Failed to map memory");
		VK_CHECK_RESULT(vkMapMemory(m_pDevice->GetHandle(), reinterpret_cast<VulkanBuffer*>(m_pBuffer)->GetInfo().deviceMemory, offset, size, 0, &pTarget), "Failed to map memory");
		return pTarget;
	}

	void VulkanStagingBuffer::Unlock()
	{
		// vmaUnmapMemory(m_pAllocator->GetHandle(), AsVulkanBuffer(m_pBuffer)->GetAllocation());
		vkUnmapMemory(m_pDevice->GetHandle(), reinterpret_cast<VulkanBuffer*>(m_pBuffer)->GetInfo().deviceMemory);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	VulkanVertexBuffer::VulkanVertexBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, uint32_t vertciesCount, uint32_t strideSize)
		: m_pDevice(pDevice)
		, m_pAllocator(pAllocator)

	{
		m_VertexCount = vertciesCount;
		m_GetStride = strideSize;

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.size  = m_GetStride * m_VertexCount;
		createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount;
		createInfo.pQueueFamilyIndices;

		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.flags = 0;
		allocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		
		m_pBuffer = new VulkanBuffer(m_pDevice, m_pAllocator, createInfo, allocationInfo);

	
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		delete m_pBuffer;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	VulkanIndexBuffer::VulkanIndexBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, uint32_t indciesCount, uint32_t strideSize)
		: m_pDevice(pDevice)
		, m_pAllocator(pAllocator)
	{
		m_IndciesCount = indciesCount;
		m_GetStride = strideSize;

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.size = m_GetStride * m_IndciesCount;
		createInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount;
		createInfo.pQueueFamilyIndices;

		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.flags = 0;
		allocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;


		m_pBuffer = new VulkanBuffer(m_pDevice, m_pAllocator, createInfo, allocationInfo);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		delete m_pBuffer;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	VulkanUniformBuffer::VulkanUniformBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, size_t bufferSize)
		: m_pDevice(pDevice)
		, m_pAllocator(pAllocator)
	{

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.size = bufferSize;
		createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount;
		createInfo.pQueueFamilyIndices;

		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.flags = 0;
		allocationInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

		m_pBuffer = new VulkanBuffer(m_pDevice, m_pAllocator, createInfo, allocationInfo);
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		delete m_pBuffer;
	}

	void VulkanUniformBuffer::SetData(size_t dataSize, const void* pData)
	{
		void* pTarget;
		VK_CHECK_RESULT(vmaMapMemory(m_pAllocator->GetHandle(), reinterpret_cast<VulkanBuffer*>(m_pBuffer)->GetAllocation(), &pTarget), "Failed to map memory");
		memcpy(pTarget, pData, dataSize);
		vmaUnmapMemory(m_pAllocator->GetHandle(), reinterpret_cast<VulkanBuffer*>(m_pBuffer)->GetAllocation());
	}


}
