#pragma once

#include "Hazel.h"

class SandboxRHI : public Hazel::Layer
{
public:
	SandboxRHI();
	virtual ~SandboxRHI() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;
private:
};
