#pragma once
#include "Hazel/RHI/RHIResources.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>

namespace Hazel {
	
	#define VK_CHECK_RESULT(result, msg) HZ_CORE_ASSERT(result == VK_SUCCESS, VulkanUtils::ResultToString(result).c_str())

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
			default:													return "UNKOWN RESULT";
			}
		}
		
		static VkFormat GetFromat(const RHIFormat format)
		{
			
			switch (format)
			{
			case RHIFormat::eSignedR32:			return VK_FORMAT_R32_SINT;
			case RHIFormat::eSignedRG32:		return VK_FORMAT_R32G32_SINT;
			case RHIFormat::eSignedRGB32:		return VK_FORMAT_R32G32B32_SINT;
			case RHIFormat::eSignedRGBA32:		return VK_FORMAT_R32G32B32A32_SINT;
			
			case RHIFormat::eSignedR16:			return VK_FORMAT_R16_SINT;
			case RHIFormat::eSignedRG16:		return VK_FORMAT_R16G16_SINT;
			case RHIFormat::eSignedRGB16:		return VK_FORMAT_R16G16B16_SINT;
			case RHIFormat::eSignedRGBA16:		return VK_FORMAT_R16G16B16A16_SINT;
			
			case RHIFormat::eSignedR8:			return VK_FORMAT_R8_SINT;
			case RHIFormat::eSignedRG8:			return VK_FORMAT_R8G8_SINT;
			case RHIFormat::eSignedRGB8:		return VK_FORMAT_R8G8B8_SINT;
			case RHIFormat::eSignedRGBA8:		return VK_FORMAT_R8G8B8A8_SINT;
			
			case RHIFormat::eUnsignedR32:		return VK_FORMAT_R32_UINT;
			case RHIFormat::eUnsignedRG32:		return VK_FORMAT_R32G32_UINT;
			case RHIFormat::eUnsignedRGB32:		return VK_FORMAT_R32G32B32_UINT;
			case RHIFormat::eUnsignedRGBA32:	return VK_FORMAT_R32G32B32A32_UINT;
			
			case RHIFormat::eUnsignedR16:		return VK_FORMAT_R16_UINT;
			case RHIFormat::eUnsignedRG16:		return VK_FORMAT_R16G16_UINT;
			case RHIFormat::eUnsignedRGB16:		return VK_FORMAT_R16G16B16_UINT;
			case RHIFormat::eUnsignedRGBA16:	return VK_FORMAT_R16G16B16A16_UINT;
			
			case RHIFormat::eUnsignedR8:		return VK_FORMAT_R8_UINT;
			case RHIFormat::eUnsignedRG8:		return VK_FORMAT_R8G8_UINT;
			case RHIFormat::eUnsignedRGB8:		return VK_FORMAT_R8G8B8_UINT;
			case RHIFormat::eUnsignedRGBA8:		return VK_FORMAT_R8G8B8A8_UINT;

			case RHIFormat::eFloatR:			return VK_FORMAT_R32_SFLOAT;
			case RHIFormat::eFloatRG:			return VK_FORMAT_R32G32_SFLOAT;
			case RHIFormat::eFloatRGB:			return VK_FORMAT_R32G32B32_SFLOAT;
			case RHIFormat::eFloatRGBA:			return VK_FORMAT_R32G32B32A32_SFLOAT;

			case RHIFormat::eDoubleR:			return VK_FORMAT_R64_SFLOAT;
			case RHIFormat::eDoubleRG:			return VK_FORMAT_R64G64_SFLOAT;
			case RHIFormat::eDoubleRGB:			return VK_FORMAT_R64G64B64_SFLOAT;
			case RHIFormat::eDoubleRGBA:		return VK_FORMAT_R64G64B64A64_SFLOAT;

			case RHIFormat::eSRGB_R8:			return VK_FORMAT_R8_SRGB;
			case RHIFormat::eSRGB_RG8:			return VK_FORMAT_R8G8_SRGB;
			case RHIFormat::eSRGB_RGB8:			return VK_FORMAT_R8G8B8_SRGB;
			case RHIFormat::eSRGB_RGBA8:		return VK_FORMAT_R8G8B8A8_SRGB;
			
			default:							return VK_FORMAT_MAX_ENUM;
			}

		}

		static size_t GetFormatSize(VkFormat format)
		{
			switch (format)
			{
			case VK_FORMAT_R32_SINT:
			case VK_FORMAT_R32_UINT:
			case VK_FORMAT_R32_SFLOAT:
				return 1 * 4;

			case VK_FORMAT_R32G32_SINT:
			case VK_FORMAT_R32G32_UINT:
			case VK_FORMAT_R32G32_SFLOAT:
				return 2 * 4;

			case VK_FORMAT_R32G32B32_SINT:
			case VK_FORMAT_R32G32B32_UINT:
			case VK_FORMAT_R32G32B32_SFLOAT:
				return 3 * 4;

			case VK_FORMAT_R32G32B32A32_SINT:
			case VK_FORMAT_R32G32B32A32_UINT:
			case VK_FORMAT_R32G32B32A32_SFLOAT:
				return 4 * 4;

			case VK_FORMAT_R16_SINT:
			case VK_FORMAT_R16_UINT:
				return 1 * 2;

			case VK_FORMAT_R16G16_SINT:
			case VK_FORMAT_R16G16_UINT:
				return 2 * 2;

			case VK_FORMAT_R16G16B16_SINT:
			case VK_FORMAT_R16G16B16_UINT:
				return 3 * 2;

			case VK_FORMAT_R16G16B16A16_SINT:
			case VK_FORMAT_R16G16B16A16_UINT:
				return 4 * 2;

			case VK_FORMAT_R8_SINT:
			case VK_FORMAT_R8_UINT:
				return 1 * 1;

			case VK_FORMAT_R8G8_SINT:
			case VK_FORMAT_R8G8_UINT:
				return 2 * 1;

			case VK_FORMAT_R8G8B8_SINT:
			case VK_FORMAT_R8G8B8_UINT:
				return 3 * 1;

			case VK_FORMAT_R8G8B8A8_SINT:
			case VK_FORMAT_R8G8B8A8_UINT:
				return 4 * 1;

			case VK_FORMAT_R64_SFLOAT:
				return 1 * 8;
			case VK_FORMAT_R64G64_SFLOAT:
				return 2 * 8;
			case VK_FORMAT_R64G64B64_SFLOAT:
				return 3 * 8;
			case VK_FORMAT_R64G64B64A64_SFLOAT:
				return 4 * 8;

			default: return SIZE_MAX;
			}
		}
		
		static VkFilter GetFilter(const RHIFilter filter)
		{
			switch (filter)
			{
			case RHIFilter::eNearest: return VK_FILTER_NEAREST;
			case RHIFilter::eLinear: return VK_FILTER_LINEAR;
			default: return VK_FILTER_MAX_ENUM;
			}
		}

		static VkSamplerMipmapMode GetMipmapMode(const RHIFilter filter)
		{
			switch (filter)
			{
			case RHIFilter::eNearest:	return	VK_SAMPLER_MIPMAP_MODE_NEAREST;
			case RHIFilter::eLinear:	return	VK_SAMPLER_MIPMAP_MODE_LINEAR;
			default:					return	VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;
			}
		}

		static VkSamplerAddressMode GetAddressMode(const RHITextureAddressMode mode)
		{
			switch (mode)
			{
			case RHITextureAddressMode::eRepeat:				return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			case RHITextureAddressMode::eMirroredRepeat:		return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			case RHITextureAddressMode::eClampToEdge:			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case RHITextureAddressMode::eClampToBorder:			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
			case RHITextureAddressMode::eMirroredClampToEdge:	return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
			default:											return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
			}
		}

		static VkCompareOp GetCompareOp(const RHIComparsionOperation op)
		{
			switch (op)
			{

			case RHIComparsionOperation::eNever:		 return VK_COMPARE_OP_NEVER;
			case RHIComparsionOperation::eLess:			 return VK_COMPARE_OP_LESS;
			case RHIComparsionOperation::eEqual:		 return VK_COMPARE_OP_EQUAL;
			case RHIComparsionOperation::eLessOrEqual:	 return VK_COMPARE_OP_LESS_OR_EQUAL;
			case RHIComparsionOperation::eGreater:		 return VK_COMPARE_OP_GREATER;
			case RHIComparsionOperation::eNotEqual:		 return VK_COMPARE_OP_NOT_EQUAL;
			case RHIComparsionOperation::eGreaterOrEqual :return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case RHIComparsionOperation::eAlways:		 return VK_COMPARE_OP_ALWAYS;
			default:									 return VK_COMPARE_OP_MAX_ENUM;
			}

		}

		static VkSampleCountFlagBits GetSampleCount(uint32_t count)
		{
			switch (count)
			{
			case 1:  return VK_SAMPLE_COUNT_1_BIT;
			case 2:  return VK_SAMPLE_COUNT_2_BIT;
			case 4:  return VK_SAMPLE_COUNT_4_BIT;
			case 8:  return VK_SAMPLE_COUNT_8_BIT;
			case 16: return VK_SAMPLE_COUNT_16_BIT;
			case 32: return VK_SAMPLE_COUNT_32_BIT;
			default: 
				HZ_CORE_ERROR("Invalid count, count must be a multiple of 2");
				return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
			}
		}

		static VkBorderColor GetBorderColor(const RHISamplerBorderColor borderColor)
		{
			switch (borderColor)
			{
			case RHISamplerBorderColor::eFloatTransparentBlack:		   return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			case RHISamplerBorderColor::eIntTransparentBlack:		   return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
			case RHISamplerBorderColor::eFloatOpaqueBlack:			   return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
			case RHISamplerBorderColor::eIntOpaqueBlack:			   return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			case RHISamplerBorderColor::eFloatOpaqueWhite:			   return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			case RHISamplerBorderColor::eIntOpaqueWhite:			   return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
			default:								                   return VK_BORDER_COLOR_MAX_ENUM;
			}
		}


		static VkSurfaceFormatKHR SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
		{
			for (const auto& availableFormat : formats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}
			return formats[0];
		}

		static VkPresentModeKHR SelectSurfacePresentMode(const std::vector<VkPresentModeKHR>& presentModes)
		{
			for (const auto& availablePresentMode : presentModes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return availablePresentMode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}

		static VkExtent2D SelectSurfaceExtent(VkExtent2D actualExtent, VkExtent2D currentExtent, VkExtent2D minImageExtent, VkExtent2D maxImageExtent)
		{
			if (actualExtent.width != UINT32_MAX)
			{
				return currentExtent;
			}
			else
			{
				actualExtent.width = std::max(minImageExtent.width, std::min(maxImageExtent.width, actualExtent.width));
				actualExtent.height = std::max(minImageExtent.height, std::min(maxImageExtent.height, actualExtent.height));

				return actualExtent;
			}
		}


	}



}
