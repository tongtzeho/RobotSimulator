#pragma once

#include "../Engine/Shader/Effect.h"

class DepthMapEffect : public CE::Effect
{
private:
	ID3DX11EffectTechnique* depthMapTech;
	ID3DX11EffectMatrixVariable* rect;
	ID3DX11EffectScalarVariable* depthSensorFar;
	ID3DX11EffectScalarVariable* near_;
	ID3DX11EffectScalarVariable* far_;
	ID3DX11EffectScalarVariable* isPerspective;
	ID3DX11EffectMatrixVariable* ndc;
	ID3DX11EffectShaderResourceVariable* depthMap;
	virtual void BuildInputLayout(ID3D11Device* device);
public:
	DepthMapEffect(ID3D11Device* device);
	virtual void SetMatrix(DirectX::CXMMATRIX matrix, const unsigned param = 0);
	virtual void SetShaderResource(ID3D11ShaderResourceView *srv, const unsigned param = 0);
	virtual void SetScalar(const void *data, const unsigned param = 0);
};