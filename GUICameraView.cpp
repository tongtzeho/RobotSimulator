#include "GUICameraView.h"
#include "../Engine/CoolEngineGame.h"

using namespace CE;

void GUICameraView::SetCameraSensor(CameraSensor *const cameraSensor, const bool isDepthMap)
{
	this->cameraSensor = cameraSensor;
	this->isDepthMap = isDepthMap;
	this->isPerspective = cameraSensor->IsPerspective();
	this->depthSensorFar = cameraSensor->GetDepthSensorFar();
	this->near_ = cameraSensor->GetNear();
	this->far_ = cameraSensor->GetFar();
	if (isDepthMap)
	{
		SetTexture(cameraSensor->GetDrawableTexture()->GetDepthTexture());
	}
	else
	{
		SetTexture(cameraSensor->GetDrawableTexture()->GetColorTexture());
	}
}

void GUICameraView::Draw(void *const param)
{
	if (!isDepthMap)
	{
		GUIImage::Draw(param);
	}
	else
	{
		CheckStarted();
		if (GetDepth() >= 0 && GetTexture() != NULL)
		{
			ID3D11DeviceContext *md3dImmediateContext = CoolEngineGame::Instance()->GetDeviceContext();
			RenderManager *renderMgr = CoolEngineGame::Instance()->GetRenderManager();
			Effect *effect = renderMgr->GetEffectAt(4);
			std::pair<Effect*, std::pair<unsigned, unsigned>> shader({ effect,{ 0, 0 } });
			CoolEngineGame::Instance()->GetGUIManager()->PrepareToDrawImage(*this, &shader);
			renderMgr->SetRasterizerStateAt(-1);
			renderMgr->SetBlendStateAt(-1);
			renderMgr->SetDepthStencilStateAt(0, (GetDepth() & 3) << 6);
			effect->SetScalar(&isPerspective, 0);
			effect->SetScalar(&depthSensorFar, 1);
			if (isPerspective)
			{
				effect->SetScalar(&near_, 2);
			}
			effect->SetScalar(&far_, 3);
			effect->GetActiveTech()->GetPassByIndex(0)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(6, 0, 0);
		}
	}
}