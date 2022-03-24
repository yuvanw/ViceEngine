#include "Frustum.h"
#include "Viewport.h"
#include "RenderManager.h"

const float FFrustum::DefaultNearDistance = 0.1f;
const float FFrustum::DefaultFarDistance = 1000.0f;
const float FFrustum::DefaultVerticalFovRadians = 45.0f;

FFrustum::FFrustum()
	: Width(FRenderManager::GetRenderer().Get()->GetViewport().Width)
	, Height(FRenderManager::GetRenderer().Get()->GetViewport().Height)
	, NearDistance(DefaultNearDistance)
	, FarDistance(DefaultFarDistance)
	, VerticalFovRadians(DefaultVerticalFovRadians)
{
}
