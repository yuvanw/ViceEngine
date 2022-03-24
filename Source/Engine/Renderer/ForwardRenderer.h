#pragma once 

#include "CoreMinimal.h"
#include "Renderer.h"
#include "RHI/Pipeline.h"
#include "RHI/VertexArray.h"

class FForwardRenderer : public FRenderer
{
public:
	FForwardRenderer() = default;

private:
	// Begin FRenderer interface.
	virtual void Render() override;
	// End FRenderer interface.

	void RenderSkybox();
	void RenderModel(const TSharedPtr<FModel>& InModel);
};
