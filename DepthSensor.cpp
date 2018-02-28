#include "DepthSensor.h"
#include "RobotSimulatorScene.h"

using namespace CE;

DepthSensor::DepthSensor(IComponent *comp, const char *cameraIdentifier, const char *param) : ICameraSensor(comp, cameraIdentifier, param)
{
	assert(GetCamera() != nullptr);
	ID3D11Resource *resource = NULL;
	GetCamera()->GetTargetTexture().GetDepthTexture()->GetShaderResourceView()->GetResource(&resource);
	textureSrc = (ID3D11Texture2D*)resource;
	D3D11_TEXTURE2D_DESC description;
	textureSrc->GetDesc(&description);
	description.BindFlags = 0;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	description.Usage = D3D11_USAGE_STAGING;
	HR(CoolEngine::Instance()->GetDevice()->CreateTexture2D(&description, NULL, &textureDest));
	height = description.Height;
	width = description.Width;
	channel = 1;
	byte = 4;
	textureData = new unsigned char[height*width*channel*byte];
	unsigned tmpU;
	int tmpD;
	int ret = sscanf(param, "%u %d %f", &tmpU, &tmpD, &farZ);
	assert(farZ > 0);
}

bool DepthSensor::Sample()
{
	if (ICameraSensor::Sample())
	{
		if (GetCamera()->IsPerspective()) // 把深度缓冲区的数值转换为深度值，再映射到0到1区间
		{
			const float cameraFarZ = GetCamera()->GetFarZ();
			const float cameraNearZ = GetCamera()->GetNearZ();
			const float denominator = cameraFarZ / (cameraFarZ - cameraNearZ);
			const float numerator = (cameraNearZ*cameraFarZ*(cameraFarZ - 1)) / ((cameraFarZ - cameraNearZ)*(cameraFarZ - cameraNearZ)*farZ);
			const float u24To01 = 1.0f / 0xFFFFFF;
			const unsigned *src = reinterpret_cast<const unsigned*>(textureData); // 在GPU里是按高8位模板低24位深度的整型存储，转换为0到1的float
			float *dest = reinterpret_cast<float*>(textureData);
			for (unsigned i = 0; i < width*height; ++i)
			{
				dest[i] = Math::Clamp(numerator / (denominator - (src[i] & 0xFFFFFF)*u24To01), 0.0f, 1.0f);
			}
		}
		else
		{
			const float numerator = GetCamera()->GetFarZ() / farZ;
			const float u24To01 = 1.0f / 0xFFFFFF;
			const unsigned *src = reinterpret_cast<const unsigned*>(textureData);
			float *dest = reinterpret_cast<float*>(textureData);
			for (unsigned i = 0; i < width*height; ++i)
			{
				dest[i] = Math::Clamp(numerator * ((src[i] & 0xFFFFFF)*u24To01), 0.0f, 1.0f);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

unsigned char* DepthSensor::GetBMPPixels() const
{
	unsigned char *result = new unsigned char[height*width * 3];
	const float *textureData = reinterpret_cast<const float*>(this->textureData);
	for (unsigned i = 0; i < width*height; ++i)
	{
		result[i * 3] = 0;
		result[i * 3 + 1] = result[i * 3 + 2] = static_cast<unsigned char>(textureData[i] * 255.0f);
	}
	return result;
}

template<> Component<DepthSensor>::Component(Entity *const belong, const char *identifier, const void *param) : IComponent(belong, identifier), data(nullptr)
{
	assert(param != nullptr);
	data = new DepthSensor(this, identifier, (const char*)param);
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->AppendCameraSensor(data);
	}
}

template<> Component<DepthSensor>::~Component()
{
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->DeleteCameraSensor(data);
	}
	if (data != nullptr)
	{
		SafeDelete(data);
	}
	GetEntity()->EraseComponent(GetComponentArrayIndex(), this);
}

template<> inline const char* Component<DepthSensor>::GetTypeName() const
{
	return "DepthSensor";
}