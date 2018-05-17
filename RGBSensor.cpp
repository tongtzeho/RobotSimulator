#include "RGBSensor.h"
#include "RobotSimulatorScene.h"

using namespace CE;

RGBSensor::RGBSensor(IComponent *comp, const char *cameraIdentifier, const char *param) : ICameraSensor(comp, cameraIdentifier, param)
{
	assert(GetCamera() != nullptr);
	ID3D11Resource *resource = NULL;
	GetCamera()->GetTargetTexture().GetColorTexture()->GetShaderResourceView()->GetResource(&resource);
	textureSrc = (ID3D11Texture2D*)resource;
	D3D11_TEXTURE2D_DESC description;
	textureSrc->GetDesc(&description);
	description.BindFlags = 0;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	description.Usage = D3D11_USAGE_STAGING;
	HR(CoolEngine::Instance()->GetDevice()->CreateTexture2D(&description, NULL, &textureDest));
	height = description.Height;
	width = description.Width;
	channel = 4;
	textureData = new unsigned char[height*width*channel*4]; // sizeof(float)=4
	sendData = new char[8 + height*width * 4];
}

unsigned char* RGBSensor::GetBMPPixels() const
{
	unsigned char *result = new unsigned char[height*width * 3];
	const float *textureData = reinterpret_cast<const float*>(this->textureData);
	for (unsigned i = 0; i < width*height; ++i)
	{
		result[i * 3] = static_cast<unsigned char>(Math::Clamp(textureData[i * 4 + 2], 0.0f, 1.0f) * 255.0f); // B
		result[i * 3 + 1] = static_cast<unsigned char>(Math::Clamp(textureData[i * 4 + 1], 0.0f, 1.0f) * 255.0f); // G
		result[i * 3 + 2] = static_cast<unsigned char>(Math::Clamp(textureData[i * 4], 0.0f, 1.0f) * 255.0f); // R
	}
	return result;
}

void RGBSensor::SendDataToSocketClient(SOCKET clientSocket)
{
	char *addr = sendData;
	memcpy(addr, &height, 4);
	addr += 4;
	memcpy(addr, &width, 4);
	addr += 4;
	const float *textureData = reinterpret_cast<const float*>(this->textureData);
	for (unsigned i = 0; i < width*height; ++i)
	{
		unsigned rgba = (static_cast<unsigned>(Math::Clamp(textureData[i * 4], 0.0f, 1.0f) * 255.0f) << 24) | (static_cast<unsigned>(Math::Clamp(textureData[i * 4 + 1], 0.0f, 1.0f) * 255.0f) << 16) | (static_cast<unsigned>(Math::Clamp(textureData[i * 4 + 2], 0.0f, 1.0f) * 255.0f) << 8) | static_cast<unsigned>(Math::Clamp(textureData[i * 4 + 3], 0.0f, 1.0f) * 255.0f);
		memcpy(addr, &rgba, 4);
		addr += 4;
	}
	ICameraSensor::SendDataToSocketClient(clientSocket);
}

template<> Component<RGBSensor>::Component(Entity *const belong, const char *identifier, const void *param) : IComponent(belong, identifier), data(nullptr)
{
	assert(param != nullptr);
	data = new RGBSensor(this, identifier, (const char*)param);
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->AppendCameraSensor(data);
	}
}

template<> Component<RGBSensor>::~Component()
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

template<> inline const char* Component<RGBSensor>::GetTypeName() const
{
	return "RGBSensor";
}