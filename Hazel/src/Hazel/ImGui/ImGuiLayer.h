#pragma once

#include "Hazel/Core/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Hazel/RHI/RendererHardwareInterface.h"

namespace Hazel {
	
	struct ImGuiDesc
	{
		RHITexture2D*					FontAtlas;
		RHISampler*						pSampler;
		RHIVertexBuffer*				pVertexBuffer;
		RHIIndexBuffer*					pIndexBuffer;
		RHIPipelineLayout*				pPipelineLayout;
		RHIGraphicsPipelineState*		pPipeline;
		RHIDescriptorSetLayoutDesc*		pDescriptorsLayout;
		RHIDescriptorPool*				pPool;
		RHIDescriptorSet*				pSet;
		void*							pNativeDevice;
	};


	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() = delete;
		ImGuiLayer(RHI* pRHI);
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

	private:
		RHI* m_pRHI;
	
	};

}
