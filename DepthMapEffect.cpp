#include "DepthMapEffect.h"

using namespace DirectX;
using namespace CE;

DepthMapEffect::DepthMapEffect(ID3D11Device* device) : Effect(device, "RobotSimulator/Shader/DepthMap.fx")
{
	depthMapTech = mFX->GetTechniqueByName("DepthMapTech");
	rect = mFX->GetVariableByName("gRect")->AsMatrix();
	ndc = mFX->GetVariableByName("gNdc")->AsMatrix();
	depthMap = mFX->GetVariableByName("gDepthMap")->AsShaderResource();
	activeTech = &depthMapTech;
	BuildInputLayout(device);
}

void DepthMapEffect::BuildInputLayout(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	(*activeTech)->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(layoutDesc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout));
}

void DepthMapEffect::SetMatrix(CXMMATRIX matrix, const unsigned param)
{
	switch (param)
	{
	case 0:
		rect->SetMatrix(reinterpret_cast<const float*>(&matrix));
		break;
	case 1:
		ndc->SetMatrix(reinterpret_cast<const float*>(&matrix));
		break;
	default:
		assert(false);
	}
}

void DepthMapEffect::SetShaderResource(ID3D11ShaderResourceView *srv, const unsigned param)
{
	assert(srv != NULL);
	depthMap->SetResource(srv);
}