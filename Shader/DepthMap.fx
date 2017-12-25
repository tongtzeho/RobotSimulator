cbuffer cbPerFrame
{
	float4x4 gNdc;
};

cbuffer cbPerObject
{
	float4x4 gRect;
};

Texture2D gDepthMap;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float2 PosPixel : POSITION;
	float2 Tex    : TEXCOORD;
	float4 Color  : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex : TEXCOORD;
};
 
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.PosH = mul(float4(vin.PosPixel, 0.0f, 1.0f), mul(gRect, gNdc));
	vout.Tex = vin.Tex;
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float r = gDepthMap.Sample(gTriLinearSam, pin.Tex).r;
	
	// draw as grayscale
	return float4(r, r, r, 1);
}
 
technique11 DepthMapTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
