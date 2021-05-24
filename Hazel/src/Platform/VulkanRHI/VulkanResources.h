#pragma once
#include "Platform/VulkanRHI/vma/vk_mem_alloc.h"
#include "Hazel/RHI/RHIResources.h"

namespace Hazel {

	class VulkanDevice;
	class VulkanMemoryAllocator;

	class VulkanBuffer;

	inline VulkanBuffer* AsVulkanBuffer(RHIBuffer* pBuffer) 
	{
		return reinterpret_cast<VulkanBuffer*>(pBuffer);
	}

	inline const VulkanBuffer* AsVulkanBuffer(const RHIBuffer* pBuffer) 
	{ 
		return reinterpret_cast<const VulkanBuffer*>(pBuffer); 
	}

	class VulkanBuffer : public RHIBuffer
	{
	public:
		VulkanBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator);
		VulkanBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, const VkBufferCreateInfo& createInfo, const VmaAllocationCreateInfo& allocInfo);
		~VulkanBuffer();

		void Init(const VkBufferCreateInfo& createInfo, const VmaAllocationCreateInfo& allocationInfo);

		inline VkBuffer GetHandle() const { return m_BufferHandle; }
		inline VmaAllocation GetAllocation() const { return m_Allocation; }
		inline VmaAllocationInfo GetInfo() const { return m_AllocationInfo; }

	private:
		const VulkanDevice*				m_pDevice;
		const VulkanMemoryAllocator*	m_pAllocator;
		VkBuffer						m_BufferHandle;
		VmaAllocation					m_Allocation;
		VmaAllocationInfo				m_AllocationInfo;
	};

	class VulkanStagingBuffer : public RHIStagingBuffer
	{
	public:
		VulkanStagingBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, size_t bufferSize);
		~VulkanStagingBuffer();

		inline VulkanBuffer* GetAsVulkanBuffer() { return reinterpret_cast<VulkanBuffer*>(m_pBuffer); }
		inline const VulkanBuffer* GetAsVulkanBuffer() const { return reinterpret_cast<const VulkanBuffer*>(m_pBuffer); }

		virtual void* Lock(size_t offset, size_t size) final override;
		virtual void Unlock() final override;

	private:
		const VulkanDevice*				m_pDevice;
		const VulkanMemoryAllocator*	m_pAllocator;
	};

	class VulkanVertexBuffer : public RHIVertexBuffer
	{
	public:
		VulkanVertexBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, uint32_t vertciesCount, uint32_t strideSize);
		~VulkanVertexBuffer();

		inline VulkanBuffer* GetAsVulkanBuffer() { return reinterpret_cast<VulkanBuffer*>(m_pBuffer); }
		inline const VulkanBuffer* GetAsVulkanBuffer() const { return reinterpret_cast<const VulkanBuffer*>(m_pBuffer); }

	private:
		const VulkanDevice*				m_pDevice;
		const VulkanMemoryAllocator*	m_pAllocator;
	};

	class VulkanIndexBuffer : public RHIIndexBuffer
	{
	public:
		VulkanIndexBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, uint32_t indciesCount, uint32_t strideSize);
		~VulkanIndexBuffer();

		inline VulkanBuffer* GetAsVulkanBuffer() { return reinterpret_cast<VulkanBuffer*>(m_pBuffer); }
		inline const VulkanBuffer* GetAsVulkanBuffer() const { return reinterpret_cast<const VulkanBuffer*>(m_pBuffer); }

	private:
		const VulkanDevice*				m_pDevice;
		const VulkanMemoryAllocator*	m_pAllocator;
	};

	class VulkanUniformBuffer : public RHIUniformBuffer
	{
	public:
		VulkanUniformBuffer(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, size_t bufferSize);
		~VulkanUniformBuffer();

		virtual void SetData(size_t dataSize, const void* pData) final override;

		inline VulkanBuffer* GetAsVulkanBuffer() { return reinterpret_cast<VulkanBuffer*>(m_pBuffer); }
		inline const VulkanBuffer* GetAsVulkanBuffer() const { return reinterpret_cast<const VulkanBuffer*>(m_pBuffer); }
	
	private:
		const VulkanDevice*				m_pDevice;
		const VulkanMemoryAllocator*	m_pAllocator;
	};

	class VulkanSampler : public RHISampler
	{
	public:
		VulkanSampler(const VulkanDevice* pDevice, const VkSamplerCreateInfo& createInfo);
		~VulkanSampler();

		inline VkSampler GetHandle() const { return m_hSampler; }

	private:
		const VulkanDevice* m_pDevice;
		VkSampler			m_hSampler;
	};

	class VulkanImage : public RHITexture
	{
	public:
		VulkanImage(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator);
		VulkanImage(const VulkanDevice* pDevice, const VulkanMemoryAllocator* pAllocator, const VkImageCreateInfo& createInfo, const VmaAllocationCreateInfo& allocInfo);

		~VulkanImage();

		void Init(const VkImageCreateInfo& createInfo, const VmaAllocationCreateInfo& allocationInfo);

		inline VkImage GetHandle() const { return m_hImage; }
		inline VmaAllocation GetAllocation() const { return m_Allocation; }
		inline VmaAllocationInfo GetInfo() const { return m_AllocationInfo; }

	private:
		const VulkanDevice* m_pDevice;
		const VulkanMemoryAllocator* m_pAllocator;
		VkImage							m_hImage;
		VmaAllocation					m_Allocation;
		VmaAllocationInfo				m_AllocationInfo;
	};

	class VulkanTexture2D : public RHITexture2D
	{
	public:
		VulkanTexture2D(const VulkanDevice* pDevice, const VulkanMemoryAllocator* m_pAllocator, const RHITexture2DDesc& desc);
		~VulkanTexture2D();

		inline VkImageView GetHandle() const { return m_hView; }
	private:
		const VulkanDevice* m_pDevice;
		const VulkanMemoryAllocator* m_pAllocator;
		VkImageView m_hView;
	};

	class VulkanSemaphore
	{
	public:
		VulkanSemaphore(const VulkanDevice* pDevice);
		~VulkanSemaphore();

		inline VkSemaphore GetHandle() const { return m_SemaphoreHandle; }

	private:
		const VulkanDevice* m_pDevice;
		VkSemaphore			m_SemaphoreHandle;

	};

	class VulkanFence : public RHIFence
	{
	public:
		VulkanFence(const VulkanDevice* pDevice);
		~VulkanFence();

		inline VkFence GetHandle() const { return m_FenceHandle; }

		virtual void Reset() final override;
		virtual void Wait() final override;
		virtual bool GetFenceStatus() final override;

	private:
		const VulkanDevice* m_pDevice;
		VkFence m_FenceHandle;

	};


}
