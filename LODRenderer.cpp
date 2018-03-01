#include "LODRenderer.h"

using namespace CE;

LODRenderer::LODRenderer(IComponent *const comp, const void *param) : IRenderer(comp), __compLowMesh(nullptr), __compHighMesh(nullptr), __compMaterial(nullptr), __compTexture(nullptr), __castShadow(true), __receiveShadow(true)
{
	std::vector<IComponent*> compsMesh;
	Entity::GetComponents<Mesh>(comp->GetEntity(), compsMesh);
	__compLowMesh = dynamic_cast<Component<Mesh>*>(compsMesh[0]);
	__compHighMesh = dynamic_cast<Component<Mesh>*>(compsMesh[1]);
	__compMaterial = dynamic_cast<Component<Material>*>(Entity::GetComponent<Material>(comp->GetEntity()));
	__compTexture = dynamic_cast<Component<Texture>*>(Entity::GetComponent<Texture>(comp->GetEntity(), "DiffuseMap"));
	int castShadow;
	int receiveShadow;
	int ret = sscanf((const char*)param, "%d %d %f", &castShadow, &receiveShadow, &distThreshold);
	assert(ret == 3);
	this->__castShadow = castShadow != 0;
	this->__receiveShadow = receiveShadow != 0;
}

IRenderer *LODRenderer::Instancing(IComponent *const comp, const void *param)
{
	return new LODRenderer(comp, param);
}

void LODRenderer::InputAssemblerSetting(const unsigned stage, void *const param) // param为true/false表示是否用高级Mesh
{
	ID3D11DeviceContext *d3d11DeviceContext = CoolEngine::Instance()->GetDeviceContext();
	d3d11DeviceContext->IASetInputLayout(Vertex<Vertex3f3f2f>::GetInputLayout());
	d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	unsigned stride = sizeof(Vertex3f3f2f);
	unsigned offset = 0;
	Mesh *mesh = *(bool*)param ? (Mesh*)**__compHighMesh : (Mesh*)**__compLowMesh;
	d3d11DeviceContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer(), &stride, &offset);
	d3d11DeviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), mesh->IsIndexByte4() ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
}

ID3DX11Effect* LODRenderer::RenderEffect(const unsigned stage, void *const param)
{
	/* 与MeshRenderer的完全一致 */
	static ID3DX11Effect *renderEffect = CoolEngine::Instance()->GetRenderManager()->GetEffectAt(0);
	static ID3DX11EffectMatrixVariable* renderEffectWorld = renderEffect->GetVariableByName("gWorld")->AsMatrix();
	static ID3DX11EffectMatrixVariable* renderEffectWorldInvTranspose = renderEffect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	static ID3DX11EffectVariable* renderEffectMaterial = renderEffect->GetVariableByName("gMaterial");
	static ID3DX11EffectScalarVariable* renderEffectReceiveShadow = renderEffect->GetVariableByName("gReceiveShadow")->AsScalar();
	static ID3DX11EffectShaderResourceVariable* renderEffectDiffuseMap = renderEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	static ID3DX11Effect *shadowMapEffect = CoolEngine::Instance()->GetRenderManager()->GetEffectAt(2);
	static ID3DX11EffectMatrixVariable* shadowMapEffectWorld = shadowMapEffect->GetVariableByName("gWorld")->AsMatrix();
	static ID3DX11EffectShaderResourceVariable* shadowMapEffectDiffuseMap = shadowMapEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	if (stage == RenderManager::RenderOpaqueObjects)
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

void LODRenderer::PreRender(std::vector<IRenderer*> __outRendererLists[], void *const param)
{
	if (IsEnabled() && __compLowMesh != nullptr && **__compLowMesh != nullptr && __compHighMesh != nullptr && **__compHighMesh != nullptr &&  __compMaterial != nullptr && **__compMaterial != nullptr && __compTexture != nullptr && **__compTexture != nullptr)
	{
		__worldMatrix = GetComponent()->GetEntity()->GetWorld<Matrix4x4>();
		if (__castShadow)
		{
			__outRendererLists[RenderManager::DrawShadowMap].push_back(this);
		}
		__outRendererLists[RenderManager::RenderOpaqueObjects].push_back(this);
	}
}

void LODRenderer::Render(const unsigned stage, const unsigned cameraMask, void *const param)
{
	if (GetGroup() & cameraMask)
	{
		bool useHighMesh = true;
		if (stage == RenderManager::DrawShadowMap)
		{
			InputAssemblerSetting(stage, &useHighMesh);
		}
		else
		{
			float cameraDistance = (Vector3(__worldMatrix._41, __worldMatrix._42, __worldMatrix._43) - CoolEngine::Instance()->GetCurrentScene()->__GetCurrentCamera()->__GetWorldPosition()).Square();
			useHighMesh = cameraDistance <= distThreshold*distThreshold;
			InputAssemblerSetting(stage, &useHighMesh);
		}
		ID3DX11Effect *effect = RenderEffect(stage, param);
		ID3D11DeviceContext *d3d11DeviceContext = CoolEngine::Instance()->GetDeviceContext();
		if (stage == RenderManager::DrawShadowMap)
		{
			effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, d3d11DeviceContext);
			d3d11DeviceContext->DrawIndexed(((Mesh*)**__compHighMesh)->GetIndexCount(), 0, 0);
		}
		else if (stage == RenderManager::RenderOpaqueObjects)
		{
			effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, d3d11DeviceContext);
			d3d11DeviceContext->DrawIndexed(useHighMesh ? ((Mesh*)**__compHighMesh)->GetIndexCount() : ((Mesh*)**__compLowMesh)->GetIndexCount(), 0, 0);
		}
	}
}