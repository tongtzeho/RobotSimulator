#pragma once

#include "../Engine/CoolEngine.h"

class LODRenderer : public CE::IRenderer
{
private:
	CE::Component<CE::Mesh> *__compLowMesh;
	CE::Component<CE::Mesh> *__compHighMesh;
	CE::Component<CE::Material> *__compMaterial;
	CE::Component<CE::Texture> *__compTexture;
	CE::Matrix4x4 __worldMatrix;
	float distThreshold;
	bool __castShadow;
	bool __receiveShadow;
	LODRenderer(CE::IComponent *const comp, const void *param = nullptr);
	virtual void InputAssemblerSetting(const unsigned stage, void *const param = nullptr);
	virtual ID3DX11Effect* RenderEffect(const unsigned stage, void *const param = nullptr);
public:
	static IRenderer *Instancing(CE::IComponent *const comp, const void *param = nullptr);
	virtual inline const char *GetRendererName() const { return "LODRenderer"; }
	virtual void PreRender(std::vector<IRenderer*> __outRendererLists[], void *const param = nullptr);
	virtual void Render(const unsigned stage, const unsigned cameraMask, void *const param = nullptr);
};