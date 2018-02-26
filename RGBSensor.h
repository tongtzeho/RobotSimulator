#pragma once

#include "../Engine/CoolEngine.h"

class RGBSensor
{
private:
	CE::Camera *camera;
	ID3D11Texture2D *textureSrc;
	ID3D11Texture2D *textureDest;
	float *textureData;
	unsigned height;
	unsigned width;
	float timeInterval; // 1/FPS
	float lastCopyTime; // ��ǰʱ���ȥ��һ�θ��Ƶ�ʱ�䳬��1/FPSʱ�����и���
	bool enabled;
	RGBSensor(const RGBSensor&) = delete;
	RGBSensor& operator=(const RGBSensor&) = delete;
	void InitTextureData();
public:
	RGBSensor(CE::IComponent *comp, const char *cameraIdentifier, const char *param);
	~RGBSensor();
	void CopyTextureData();
	inline bool IsEnabled() const { return enabled; }
	void SetEnabled(const bool enabled) { this->enabled = enabled; }
	inline const float *GetTextureData(unsigned &outHeight, unsigned &outWidth) const
	{
		outHeight = height;
		outWidth = width;
		return textureData;
	}
	bool SaveBMP(const char *bmpFile) const; // �ѵ�ǰ��textureData����ΪbmpͼƬ������ʹ�ã�
};