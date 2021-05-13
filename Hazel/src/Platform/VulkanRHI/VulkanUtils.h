#pragma once
#include "Hazel/RHI/RHIResources.h"

#include <vulkan/vulkan.h>

namespace Hazel {

	namespace VulkanUtils
	{
		static std::string ResultToString(VkResult result)
		{
			switch (result)
			{
			case VK_SUCCESS:											return "VK_SUCCESS";
			case VK_NOT_READY:											return "VK_NOT_READY";
			case VK_TIMEOUT:											return "VK_TIMEOUT";
			case VK_EVENT_SET:											return "VK_EVENT_SET";
			case VK_EVENT_RESET:										return "VK_EVENT_RESET";
			case VK_INCOMPLETE:											return "VK_INCOMPLETE";
			case VK_ERROR_OUT_OF_HOST_MEMORY:							return "VK_ERROR_OUT_OF_HOST_MEMORY";
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:							return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
			case VK_ERROR_INITIALIZATION_FAILED:						return "VK_ERROR_INITIALIZATION_FAILED";
			case VK_ERROR_DEVICE_LOST:									return "VK_ERROR_DEVICE_LOST";
			case VK_ERROR_MEMORY_MAP_FAILED:							return "VK_ERROR_MEMORY_MAP_FAILED";
			case VK_ERROR_LAYER_NOT_PRESENT:							return "VK_ERROR_LAYER_NOT_PRESENT";
			case VK_ERROR_EXTENSION_NOT_PRESENT:						return "VK_ERROR_EXTENSION_NOT_PRESENT";
			case VK_ERROR_FEATURE_NOT_PRESENT:							return "VK_ERROR_FEATURE_NOT_PRESENT";
			case VK_ERROR_INCOMPATIBLE_DRIVER:							return "VK_ERROR_INCOMPATIBLE_DRIVER";
			case VK_ERROR_TOO_MANY_OBJECTS:								return "VK_ERROR_TOO_MANY_OBJECTS";
			case VK_ERROR_FORMAT_NOT_SUPPORTED:							return "VK_ERROR_FORMAT_NOT_SUPPORTED";
			case VK_ERROR_FRAGMENTED_POOL:								return "VK_ERROR_FRAGMENTED_POOL";
			case VK_ERROR_UNKNOWN:										return "VK_ERROR_UNKNOWN";
			case VK_ERROR_OUT_OF_POOL_MEMORY:							return "VK_ERROR_OUT_OF_POOL_MEMORY";
			case VK_ERROR_INVALID_EXTERNAL_HANDLE:						return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
			case VK_ERROR_FRAGMENTATION:								return "VK_ERROR_FRAGMENTATION";
			case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:				return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
			case VK_ERROR_SURFACE_LOST_KHR:								return "VK_ERROR_SURFACE_LOST_KHR";
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:						return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
			case VK_SUBOPTIMAL_KHR:										return "VK_SUBOPTIMAL_KHR";
			case VK_ERROR_OUT_OF_DATE_KHR:								return "VK_ERROR_OUT_OF_DATE_KHR";
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:						return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
			case VK_ERROR_VALIDATION_FAILED_EXT:						return "VK_ERROR_VALIDATION_FAILED_EXT";
			case VK_ERROR_INVALID_SHADER_NV:							return "VK_ERROR_INVALID_SHADER_NV";
			case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
			case VK_ERROR_NOT_PERMITTED_EXT:							return "VK_ERROR_NOT_PERMITTED_EXT";
			case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:			return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
			case VK_THREAD_IDLE_KHR:									return "VK_THREAD_IDLE_KHR";
			case VK_THREAD_DONE_KHR:									return "VK_THREAD_DONE_KHR";
			case VK_OPERATION_DEFERRED_KHR:								return "VK_OPERATION_DEFERRED_KHR";
			case VK_OPERATION_NOT_DEFERRED_KHR:							return "VK_OPERATION_NOT_DEFERRED_KHR";
			case VK_PIPELINE_COMPILE_REQUIRED_EXT:						return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
			// case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:					return "VK_ERROR_OUT_OF_POOL_MEMORY_KHR";
			// case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR:				return "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR";
			// case VK_ERROR_FRAGMENTATION_EXT:							return "VK_ERROR_FRAGMENTATION_EXT";
			// case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:				return "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT";
			// case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR:		return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR";
			// case VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT:				return "VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT";
			case VK_RESULT_MAX_ENUM:									return "VK_RESULT_MAX_ENUM";
			default:													
				return "UNKOWN RESULT";
			}
		}
		
		static VkFormat GetFromat(const RHIFormat format)
		{
			switch (format)
			{
			case RHIFormat::eFloat:  return VK_FORMAT_R32_SFLOAT;
			case RHIFormat::eFloat2: return VK_FORMAT_R32G32_SFLOAT;
			case RHIFormat::eFloat3: return VK_FORMAT_R32G32B32_SFLOAT;
			case RHIFormat::eFloat4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		
			case RHIFormat::eInt:  return VK_FORMAT_R32_SINT;
			case RHIFormat::eInt2: return VK_FORMAT_R32G32_SINT;
			case RHIFormat::eInt3: return VK_FORMAT_R32G32B32_SINT;
			case RHIFormat::eInt4: return VK_FORMAT_R32G32B32A32_SINT;
		
			case RHIFormat::eUint:  return VK_FORMAT_R32_UINT;
			case RHIFormat::eUint2: return VK_FORMAT_R32G32_UINT;
			case RHIFormat::eUint3: return VK_FORMAT_R32G32B32_UINT;
			case RHIFormat::eUint4: return VK_FORMAT_R32G32B32A32_UINT;
			}

			return VK_FORMAT_UNDEFINED;
		}
		
		static size_t FormatSize(const VkFormat format)
		{
			switch (format)
			{
			case VK_FORMAT_R32_SFLOAT:			return 1 * sizeof(float);
			case VK_FORMAT_R32G32_SFLOAT:		return 2 * sizeof(float);
			case VK_FORMAT_R32G32B32_SFLOAT:	return 3 * sizeof(float);
			case VK_FORMAT_R32G32B32A32_SFLOAT: return 4 * sizeof(float);

			case VK_FORMAT_R32_SINT:			return 1 * sizeof(int32_t);
			case VK_FORMAT_R32G32_SINT:			return 2 * sizeof(int32_t);
			case VK_FORMAT_R32G32B32_SINT:		return 3 * sizeof(int32_t);
			case VK_FORMAT_R32G32B32A32_SINT:	return 4 * sizeof(int32_t);

			case VK_FORMAT_R32_UINT:			return 1 * sizeof(uint32_t);
			case VK_FORMAT_R32G32_UINT:			return 2 * sizeof(uint32_t);
			case VK_FORMAT_R32G32B32_UINT:		return 3 * sizeof(uint32_t);
			case VK_FORMAT_R32G32B32A32_UINT:	return 4 * sizeof(uint32_t);
			}

			return SIZE_MAX;
		}
		

	}

	#define VK_CHECK_RESULT(result, msg) HZ_CORE_ASSERT(result == VK_SUCCESS, VulkanUtils::ResultToString(result).c_str())

}
