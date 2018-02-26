#include "RGBSensor.h"
#include "RobotSimulatorScene.h"

using namespace CE;

RGBSensor::RGBSensor(IComponent *comp, const char *cameraIdentifier, const char *param) : camera(nullptr), textureSrc(0), textureDest(0), textureData(nullptr), height(0), width(0), lastCopyTime(0), enabled(false)
{
	Component<Camera> *compCamera = dynamic_cast<Component<Camera>*>(Entity::GetComponent<Camera>(comp->GetEntity(), cameraIdentifier));
	if (compCamera != nullptr)
	{
		camera = (Camera*)**compCamera;
		InitTextureData();
	}
	unsigned FPS;
	int enabled;
	sscanf(param, "%u %d", &FPS, &enabled);
	timeInterval = 1.0f / max(FPS, 1);
	this->enabled = enabled != 0;
}

void RGBSensor::InitTextureData()
{
	assert(camera != nullptr);
	ID3D11Resource *resource = NULL;
	camera->GetTargetTexture().GetColorTexture()->GetShaderResourceView()->GetResource(&resource);
	textureSrc = (ID3D11Texture2D*)resource;
	D3D11_TEXTURE2D_DESC description;
	textureSrc->GetDesc(&description);
	description.BindFlags = 0;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	description.Usage = D3D11_USAGE_STAGING;
	HR(CoolEngine::Instance()->GetDevice()->CreateTexture2D(&description, NULL, &textureDest));
	height = description.Height;
	width = description.Width;
	textureData = new float[height*width * 4];
}

RGBSensor::~RGBSensor()
{
	ReleaseCOM(textureDest);
	if (textureData)
	{
		delete[]textureData;
	}
}

void RGBSensor::CopyTextureData()
{
	if (enabled && textureSrc && textureDest && camera != nullptr && textureData != nullptr)
	{
		float currTime = static_cast<float>(CoolEngine::Instance()->GetTime());
		if (currTime - lastCopyTime >= timeInterval)
		{
			lastCopyTime = currTime;
			CoolEngine::Instance()->GetDeviceContext()->CopyResource(textureDest, textureSrc);
			D3D11_MAPPED_SUBRESOURCE mapped;
			HR(CoolEngine::Instance()->GetDeviceContext()->Map(textureDest, 0, D3D11_MAP_READ, 0, &mapped));
			const unsigned pitch = mapped.RowPitch;
			unsigned char *source = reinterpret_cast<unsigned char*>(mapped.pData);
			unsigned j = 0;
			for (unsigned i = 0; i < height; ++i)
			{
				memcpy(&textureData[j], source, width * 4 * 4);
				source += pitch;
				j += width * 4;
			}
			CoolEngine::Instance()->GetDeviceContext()->Unmap(textureDest, 0);
		}
	}
}

bool RGBSensor::SaveBMP(const char *bmpFile) const
{
	if (textureData == nullptr || bmpFile == nullptr || !enabled)
	{
		return false;
	}
	else
	{
		const unsigned size = height*width * 3;

		BITMAPFILEHEADER fileHeader;
		fileHeader.bfType = 0x4D42;
		fileHeader.bfReserved1 = 0;
		fileHeader.bfReserved2 = 0;
		fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;
		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		BITMAPINFOHEADER bitmapHeader = { 0 };
		bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapHeader.biHeight = -static_cast<int>(height);
		bitmapHeader.biWidth = width;
		bitmapHeader.biPlanes = 1;
		bitmapHeader.biBitCount = 24;
		bitmapHeader.biSizeImage = size;
		bitmapHeader.biCompression = 0;

		FILE *fout = fopen(bmpFile, "wb");
		if (fout == NULL)
		{
			return false;
		}
		else
		{
			fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fout);
			fwrite(&bitmapHeader, sizeof(BITMAPINFOHEADER), 1, fout);
			unsigned char *bmpData = new unsigned char[size];
			for (unsigned i = 0; i < width*height; ++i)
			{
				bmpData[i * 3] = static_cast<unsigned char>(Math::Clamp(textureData[i * 4 + 2], 0.0f, 1.0f) * 255.0f); // B
				bmpData[i * 3 + 1] = static_cast<unsigned char>(Math::Clamp(textureData[i * 4 + 1], 0.0f, 1.0f) * 255.0f); // G
				bmpData[i * 3 + 2] = static_cast<unsigned char>(Math::Clamp(textureData[i * 4 + 0], 0.0f, 1.0f) * 255.0f); // R
			}
			fwrite(bmpData, size, 1, fout);
			fclose(fout);
			delete[]bmpData;
			return true;
		}
	}
}

template<> Component<RGBSensor>::Component(Entity *const belong, const char *identifier, const void *param) : IComponent(belong, identifier), data(nullptr)
{
	assert(param != nullptr);
	data = new RGBSensor(this, identifier, (const char*)param);
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->AppendRGBSensor(data);
	}
}

template<> Component<RGBSensor>::~Component()
{
	RobotSimulatorScene *simScene = dynamic_cast<RobotSimulatorScene*>(CoolEngine::Instance()->GetCurrentScene());
	if (simScene != nullptr)
	{
		simScene->DeleteRGBSensor(data);
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