#pragma once

#include "../Engine/CoolEngine.h"

class ICameraSensor // ��Ҫʹ��Camera����Ĵ�����Interface����RGBSensor��DepthSensor����������
{
private:
	CE::Camera *camera;
protected:
	ID3D11Texture2D *textureSrc;
	ID3D11Texture2D *textureDest;
	unsigned char *textureData;
	unsigned height;
	unsigned width;
	unsigned channel; // RGBSensor��4��Channel��DepthSensor��1��Channel
	float samplePeriod; // �����ʵĵ����������ʱ�ʾһ���в����Ĵ����������ǰʱ����ϴβ�����ʱ��ļ��С�ڲ����ʵĵ������򲻲���
	float lastSampleTime; // �ϴβ�����ʱ��
	bool enabled;
	ICameraSensor(const ICameraSensor&) = delete;
	ICameraSensor& operator=(const ICameraSensor&) = delete;
	inline CE::Camera* GetCamera() const { return camera; }
	virtual unsigned char* GetBMPPixels() const = 0; // ����textureData����BMP������
public:
	ICameraSensor(CE::IComponent *comp, const char *cameraIdentifier, const char *param);
	virtual ~ICameraSensor();
	virtual const char *GetSensorType() const = 0;
	void PreRender(); // ��ÿ����Ⱦǰ�����enabled��camera��depthΪ���������camera��depth��Ϊ0
	virtual bool Sample(); // ��Texture��ÿ��Pixel��GPU���Ƶ�CPU��������textureData
	inline bool IsEnabled() const { return enabled; }
	inline void SetEnabled(const bool enabled) { this->enabled = enabled; }
	inline const float *GetTextureData(unsigned &outHeight, unsigned &outWidth, unsigned &outChannel)
	{
		outHeight = height;
		outWidth = width;
		outChannel = channel;
		return reinterpret_cast<const float*>(textureData);
	}
	bool SaveBMP(const char *bmpFile) const; // �ѵ�ǰ��textureData����ΪbmpͼƬ������ʹ�ã������������GetBMPPixels����
};