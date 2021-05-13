#pragma once
#include "RHIObject.h"

#include "Hazel/RHI/RHIResources.h"

namespace Hazel {

	struct RHIVertexBufferLayout;
	class RHIShader;
	class RHIPipelineLayout;
	// Object creation description 

	struct RHIGraphicsPipelineStateDesc
	{
		RHIShader* pVS;
		RHIShader* pPS;

		RHIVertexBufferLayout* pVertexLayout;
		RHIPipelineLayout* pLayout;
	};

	struct RHIComputePipelineStateDesc
	{

	};

	struct RHIRayTracingPipelineStateDesc
	{

	};

	// Pipeline layout
	class RHIPipelineLayout : public RHIObject
	{
	public:

	};
	
	// A base class for pipeline layouts
	class RHIPipelineState : public RHIObject
	{
	public:

	};

	// Pipelines
	class RHIGraphicsPipelineState : public RHIPipelineState
	{
	public:

	};

	class RHIComputePipelineState : public RHIPipelineState
	{
	public:

	};

	class RHIRayTracingPipelineState : public RHIPipelineState
	{
	public:

	};

}
