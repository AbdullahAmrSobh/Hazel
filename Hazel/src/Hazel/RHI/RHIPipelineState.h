#pragma once
#include "Hazel/RHI/RHIFrameBuffer.h"
#include "Hazel/RHI/RHIPipelineLayout.h"

namespace Hazel {
	
	class RHIShader;
	
	// Pipeline State Description Structures
	struct RHIVertexAttributeDesc
	{
		RHIVertexAttributeDesc(std::string name, RHIFormat format)
			: Name(name), Offset(0), Size(GetFormatSize(format)), Format(format)
		{

		}

		std::string Name;
		size_t		Offset;
		RHIFormat	Format;
		size_t		Size;
	};

	struct RHIVertexInputStateDesc
	{
		RHIVertexInputStateDesc() = default;
		RHIVertexInputStateDesc(std::initializer_list<RHIVertexAttributeDesc> attributes)
			: Attributes(attributes), Stride(0)
		{
			for (auto& attribute : Attributes)
			{
				attribute.Offset = Stride;
				Stride += attribute.Size;
			}
		}

		std::vector<RHIVertexAttributeDesc> Attributes;
		size_t Stride;
	};

	struct RHIViewport
	{
		uint32_t Width, Height;
		uint32_t OffsetX, OffsetY;
		float Min, Max;
	};

	struct RHIScissor
	{
		uint32_t Width, Height;
		uint32_t OffsetX, OffsetY;
	};

	struct RHIViewportState
	{
		RHIViewport	Viewport;
		RHIScissor	Scissor;
	};

	// Pipeline State

	struct RHIGraphicsPipelineStateDesc
	{
		RHIVertexInputStateDesc*	pVertexInputStateDesc;
		RHIViewportState*			pViewportStateDesc;

		RHIPipelineLayout*			pPipelineLayout;

		Ref<RHIShader>				pVertexShader;
		Ref<RHIShader>				pPixelShader;
	};

	class RHIGraphicsPipelineState
	{
	public:
		RHIGraphicsPipelineState() = default;
		virtual ~RHIGraphicsPipelineState() = default;

		inline Ref<RHIPipelineLayout> GetLayout() { return m_pLayout; }

	protected:
		Ref<RHIPipelineLayout> m_pLayout;
	};
}
