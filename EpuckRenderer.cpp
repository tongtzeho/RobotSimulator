#include "EpuckRenderer.h"

using namespace CE;

EpuckRenderer::EpuckRenderer(IComponent *const comp, const void *param) : IRenderer(comp), __compMesh(nullptr), __compMaterial(nullptr), __compTexture(nullptr), __castShadow(true), __receiveShadow(true)
{
	__compMesh = dynamic_cast<Component<Mesh>*>(Entity::GetComponent<Mesh>(comp->GetEntity()));
	__compMaterial = dynamic_cast<Component<Material>*>(Entity::GetComponent<Material>(comp->GetEntity()));
	__compTexture = dynamic_cast<Component<Texture>*>(Entity::GetComponent<Texture>(comp->GetEntity(), "DiffuseMap"));
	if (param != nullptr && ((const char*)param)[0] != '\0')
	{
		int castShadow;
		int receiveShadow;
		int ret = sscanf((const char*)param, "%d %d", &castShadow, &receiveShadow);
		assert(ret == 2);
		this->__castShadow = castShadow != 0;
		this->__receiveShadow = receiveShadow != 0;
	}
}

IRenderer *EpuckRenderer::Instancing(IComponent *const comp, const void *param)
{
	return new EpuckRenderer(comp, param);
}

void EpuckRenderer::InputAssemblerSetting(const unsigned stage, void *const param)
{
	ID3D11DeviceContext *d3d11DeviceContext = CoolEngine::Instance()->GetDeviceContext();
	d3d11DeviceContext->IASetInputLayout(Vertex<Vertex3f3f2f>::GetInputLayout());
	d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	unsigned stride = sizeof(Vertex3f3f2f);
	unsigned offset = 0;
	d3d11DeviceContext->IASetVertexBuffers(0, 1, ((Mesh*)**__compMesh)->GetVertexBuffer(), &stride, &offset);
	d3d11DeviceContext->IASetIndexBuffer(((Mesh*)**__compMesh)->GetIndexBuffer(), ((Mesh*)**__compMesh)->IsIndexByte4() ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
}

ID3DX11Effect* EpuckRenderer::RenderEffect(const unsigned stage, void *const param)
{
	/* 设置Epuck的Shader，传参与默认Scene几乎一样 */
	static ID3DX11Effect *renderEffect = CoolEngine::Instance()->GetRenderManager()->GetEffectByName("$Epuck");
	static ID3DX11EffectMatrixVariable* renderEffectWorld = renderEffect->GetVariableByName("gWorld")->AsMatrix();
	static ID3DX11EffectMatrixVariable* renderEffectWorldInvTranspose = renderEffect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	static ID3DX11EffectVariable* renderEffectMaterial = renderEffect->GetVariableByName("gMaterial");
	static ID3DX11EffectScalarVariable* renderEffectReceiveShadow = renderEffect->GetVariableByName("gReceiveShadow")->AsScalar();
	static ID3DX11EffectShaderResourceVariable* renderEffectDiffuseMap = renderEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	static ID3DX11Effect *shadowMapEffect = CoolEngine::Instance()->GetRenderManager()->GetEffectAt(2);
	static ID3DX11EffectMatrixVariable* shadowMapEffectWorld = shadowMapEffect->GetVariableByName("gWorld")->AsMatrix();
	static ID3DX11EffectShaderResourceVariable* shadowMapEffectDiffuseMap = shadowMapEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	if (stage == RenderManager::RenderOpaqueObjects || stage == RenderManager::RenderTransparentObjects)
	{
		renderEffectWorld->SetMatrix(__worldMatrix.m[0]);
		renderEffectWorldInvTranspose->SetMatrix(__worldMatrix.GetInverseTranspose().m[0]);
		renderEffectMaterial->SetRawValue(**__compMaterial, 0, sizeof(Material));
		renderEffectReceiveShadow->SetBool(__receiveShadow);
		renderEffectDiffuseMap->SetResource(((Texture*)**__compTexture)->GetShaderResourceView());
		return renderEffect;
	}

	else // DrawShadowMap
	{
		shadowMapEffectWorld->SetMatrix(__worldMatrix.m[0]);
		shadowMapEffectDiffuseMap->SetResource(((Texture*)**__compTexture)->GetShaderResourceView());
		return shadowMapEffect;
	}
}

void EpuckRenderer::PreRender(std::vector<IRenderer*> __outRendererLists[], void *const param)
{
	if (IsEnabled() && __compMesh != nullptr && **__compMesh != nullptr &&  __compMaterial != nullptr && **__compMaterial != nullptr && __compTexture != nullptr && **__compTexture != nullptr)
	{
		__worldMatrix = GetComponent()->GetEntity()->GetWorld<Matrix4x4>();
		if (__castShadow)
		{
			__outRendererLists[RenderManager::DrawShadowMap].push_back(this);
		}
		__outRendererLists[RenderManager::RenderOpaqueObjects].push_back(this);
		__outRendererLists[RenderManager::RenderTransparentObjects].push_back(this);
	}
}

void EpuckRenderer::Render(const unsigned stage, const unsigned cameraMask, void *const param)
{
	const unsigned transparentIndex = 2367;
	if (GetGroup() & cameraMask)
	{
		InputAssemblerSetting(stage, param);
		ID3DX11Effect *effect = RenderEffect(stage, param);
		ID3D11DeviceContext *d3d11DeviceContext = CoolEngine::Instance()->GetDeviceContext();
		if (stage == RenderManager::DrawShadowMap)
		{
			effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, d3d11DeviceContext);
			d3d11DeviceContext->DrawIndexed(((Mesh*)**__compMesh)->GetIndexCount(), 0, 0);
		}
		else if (stage == RenderManager::RenderOpaqueObjects)
		{
			effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, d3d11DeviceContext);
			d3d11DeviceContext->DrawIndexed(transparentIndex, 0, 0);
		}
		else
		{
			effect->GetTechniqueByIndex(1)->GetPassByIndex(0)->Apply(0, d3d11DeviceContext);
			d3d11DeviceContext->DrawIndexed(((Mesh*)**__compMesh)->GetIndexCount() - transparentIndex, transparentIndex, 0);
		}
	}
}