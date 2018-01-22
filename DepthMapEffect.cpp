#include "DepthMapEffect.h"

using namespace DirectX;
using namespace CE;

DepthMapEffect::DepthMapEffect(ID3D11Device* device) : Effect(device, "$DepthMap.fx")
{
	depthMapTech = fx->GetTechniqueByName("DepthMapTech");
	rect = fx->GetVariableByName("gRect")->AsMatrix();
	denominator = fx->GetVariableByName("gDenominator")->AsScalar();
	numerator = fx->GetVariableByName("gNumerator")->AsScalar();
	isPerspective = fx->GetVariableByName("gIsPerspective")->AsScalar();
	ndc = fx->GetVariableByName("gNdc")->AsMatrix();
	depthMap = fx->GetVariableByName("gDepthMap")->AsShaderResource();
	activeTech = &depthMapTech;
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
	assert(srv != 0);
	depthMap->SetResource(srv);
}

void DepthMapEffect::SetScalar(const void *data, const unsigned param)
{
	assert(data != nullptr);
	switch (param)
	{
	case 0:
		isPerspective->SetBool(*(bool*)data);
		break;
	case 1:
		denominator->SetFloat(*(float*)data);
		break;
	case 2:
		numerator->SetFloat(*(float*)data);
		break;
	default:
		assert(false);
	}
}