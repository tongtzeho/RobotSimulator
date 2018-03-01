struct Light
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Position;
	float Range;
	float3 Direction;
	float Spot;
	float3 Att;
	int Type;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

static const float SHADOW_EPSILON = 0.005f;
static const float SMAP_SIZE = 4096.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;

void ComputeDirectionalLight(Material mat, Light L, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 lightVec = -L.Direction;
	ambient = mat.Ambient * L.Ambient;	
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);			
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}

void ComputePointLight(Material mat, Light L, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 lightVec = L.Position - pos;
	float d = length(lightVec);

	if (d > L.Range)
		return;

	lightVec /= d; 
	ambient = mat.Ambient * L.Ambient;	
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);	
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));
	diffuse *= att;
	spec *= att;
}

void ComputeSpotLight(Material mat, Light L, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 lightVec = L.Position - pos;
	float d = length(lightVec);

	if( d > L.Range )
		return;

	lightVec /= d; 
	ambient = mat.Ambient * L.Ambient;	
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));
	ambient *= spot;
	diffuse *= att;
	spec *= att;
}
 
cbuffer cbPerFrame
{
	Light gLights[16];
	int gLightCount;
	float3 gCameraPos;
	float4x4 gViewProj;
	float4x4 gLightViewProj;
	bool gSceneCastShadow;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	Material gMaterial;
	bool gWithAnimation;
	bool gReceiveShadow;
}; 

cbuffer cbSkinned
{
	float4x4 gBoneTransforms[256];
};

Texture2D gDiffuseMap;
Texture2D gShadowMap;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 16;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samTriPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

RasterizerState DefaultRasterizerState {};

BlendState DefaultBlendState {};

BlendState TransparentBlendState
{
	BlendEnable[0] = TRUE;
	SrcBlend = BLEND_FACTOR;
	DestBlend = INV_BLEND_FACTOR;
	BlendOp = ADD;
	SrcBlendAlpha = BLEND_FACTOR;
	DestBlendAlpha = INV_BLEND_FACTOR;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState DefaultDepthStencilState {};

struct VertexIn
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD0;
	float4 TexProj : TEXCOORD1;
};

float ShadowFactor(float4 TexProj)
{
	TexProj.xyz /= TexProj.w;

	if (TexProj.x < -1.0f || TexProj.x > 1.0f || TexProj.y < -1.0f || TexProj.y > 1.0f || TexProj.z < 0.0f)
		return 0.0f;

	TexProj.x = +0.5f*TexProj.x + 0.5f;
	TexProj.y = -0.5f*TexProj.y + 0.5f;

	float depth = TexProj.z;

	float s0 = gShadowMap.Sample(samTriPoint, TexProj.xy).r;
	float s1 = gShadowMap.Sample(samTriPoint, TexProj.xy + float2(SMAP_DX, 0)).r;
	float s2 = gShadowMap.Sample(samTriPoint, TexProj.xy + float2(0, SMAP_DX)).r;
	float s3 = gShadowMap.Sample(samTriPoint, TexProj.xy + float2(SMAP_DX, SMAP_DX)).r;

	float result0 = depth <= s0 + SHADOW_EPSILON;
	float result1 = depth <= s1 + SHADOW_EPSILON;
	float result2 = depth <= s2 + SHADOW_EPSILON;
	float result3 = depth <= s3 + SHADOW_EPSILON;	

	float2 texelPos = SMAP_SIZE*TexProj.xy;

	float2 t = frac( texelPos );

	return lerp(lerp(result0, result1, t.x), lerp(result2, result3, t.x), t.y);
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.PosH = mul(float4(vin.PosL, 1.0f), mul(gWorld, gViewProj));
	
	if (gSceneCastShadow && gReceiveShadow)
		vout.TexProj = mul(float4(vin.PosL, 1.0f), mul(gWorld, gLightViewProj));

	vout.Tex = vin.Tex;

	return vout;
}
 
float4 PS(VertexOut pin) : SV_Target
{
	pin.NormalW = normalize(pin.NormalW);
	float3 toEye = gCameraPos - pin.PosW;
	float distToEye = length(toEye); 
	toEye /= distToEye;
	float4 texColor = float4(1, 1, 1, 1);
	texColor = gDiffuseMap.Sample( samAnisotropic, pin.Tex );
	float shadowFactor = 1;
	if (gSceneCastShadow && gReceiveShadow)
		shadowFactor = ShadowFactor(pin.TexProj);

	float4 litColor = texColor;
	if (gLightCount > 0)
	{  
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec	= float4(0.0f, 0.0f, 0.0f, 0.0f);
		
		[unroll]
		for(int i = 0; i < gLightCount; ++i)
		{
			float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
			
			if( gLights[i].Type > 0)
			{
				if (gLights[i].Type == 1)
				{
					ComputeDirectionalLight(gMaterial, gLights[i], pin.NormalW, toEye, A, D, S);
				}
				else if (gLights[i].Type == 2)
				{
					ComputePointLight(gMaterial, gLights[i], pin.PosW, pin.NormalW, toEye, A, D, S);
				}
				else if (gLights[i].Type == 3)
				{
					ComputeSpotLight(gMaterial, gLights[i], pin.PosW, pin.NormalW, toEye, A, D, S);
				}
			}
			
			ambient += A;
			diffuse += shadowFactor * D;
			spec += shadowFactor * S;
		}
		litColor = texColor*(ambient + diffuse) + spec;
	}

	litColor.a = gMaterial.Diffuse.a * texColor.a;

	return litColor;
}

technique11 OpaqueTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS() ) );
		SetRasterizerState( DefaultRasterizerState );
		SetBlendState( DefaultBlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF );
		SetDepthStencilState( DefaultDepthStencilState, 0 );
	}
}

technique11 TransparentTech
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader(ps_5_0, PS() ) );
		SetRasterizerState( DefaultRasterizerState );
		SetBlendState( TransparentBlendState, float4(0.86f, 0.86f, 0.86f, 0.86f), 0xFFFFFFFF );
		SetDepthStencilState( DefaultDepthStencilState, 0 );
	}
}