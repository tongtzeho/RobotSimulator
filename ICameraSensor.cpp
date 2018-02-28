#include "ICameraSensor.h"

using namespace CE;

ICameraSensor::ICameraSensor(CE::IComponent *comp, const char *cameraIdentifier, const char *param) : camera(nullptr), textureSrc(0), textureDest(0), textureData(nullptr), height(0), width(0), enabled(false), byte(0)
{
	Component<Camera> *compCamera = dynamic_cast<Component<Camera>*>(Entity::GetComponent<Camera>(comp->GetEntity(), cameraIdentifier));
	if (compCamera != nullptr)
	{
		camera = (Camera*)**compCamera;
	}
	unsigned sampleRate;
	int enabled;
	int ret = sscanf(param, "%u %d", &sampleRate, &enabled);
	assert(ret >= 2);
	samplePeriod = 1.0f / max(sampleRate, 1);
	this->enabled = enabled != 0;
}

ICameraSensor::~ICameraSensor()
{
	ReleaseCOM(textureDest);
	if (textureData)
	{
		delete[]textureData;
	}
}

void ICameraSensor::PreRender()
{
	if (enabled && textureSrc && textureDest && camera != nullptr && textureData != nullptr && camera->GetDepth() < 0)
	{
		camera->SetDepth(0);
	}
}

bool ICameraSensor::Sample()
{
	if (enabled && textureSrc && textureDest && camera != nullptr && textureData != nullptr)
	{
		float currTime = static_cast<float>(CoolEngine::Instance()->GetTime());
		if (currTime - lastSampleTime >= samplePeriod)
		{
			lastSampleTime = currTime;
			CoolEngine::Instance()->GetDeviceContext()->CopyResource(textureDest, textureSrc);
			D3D11_MAPPED_SUBRESOURCE mapped;
			HR(CoolEngine::Instance()->GetDeviceContext()->Map(textureDest, 0, D3D11_MAP_READ, 0, &mapped));
			const unsigned pitch = mapped.RowPitch;
			unsigned char *source = reinterpret_cast<unsigned char*>(mapped.pData);
			unsigned j = 0;
			for (unsigned i = 0; i < height; ++i)
			{
				memcpy(textureData + (j * byte), source, width * channel * byte);
				source += pitch;
				j += width * channel;
			}
			CoolEngine::Instance()->GetDeviceContext()->Unmap(textureDest, 0);
			return true;
		}
	}
	return false;
}

bool ICameraSensor::SaveBMP(const char *bmpFile) const
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
			unsigned char *bmpPixels = GetBMPPixels();
			fwrite(bmpPixels, size, 1, fout);
			fclose(fout);
			delete[]bmpPixels;
			return true;
		}
	}
}