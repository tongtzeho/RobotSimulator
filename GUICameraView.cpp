#include "GUICameraView.h"
#include "../Engine/CoolEngineGame.h"

using namespace CE;

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
			std::pair<Effect*, std::pair<unsigned, unsigned>> shader({ renderMgr->GetEffectAt(4),{ 0, 0 } });
			CoolEngineGame::Instance()->GetGUIManager()->PrepareToDrawImage(*this, &shader);
			renderMgr->SetRasterizerStateAt(-1);
			renderMgr->SetBlendStateAt(-1);
			renderMgr->SetDepthStencilStateAt(0, (GetDepth() & 3) << 6);
			renderMgr->GetEffectAt(4)->GetActiveTech()->GetPassByIndex(0)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(6, 0, 0);
		}
	}
}