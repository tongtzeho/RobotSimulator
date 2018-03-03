#pragma once

#include "../Engine/CoolEngine.h"

class ICameraSensor // 需要使用Camera组件的传感器Interface，有RGBSensor和DepthSensor两个派生类
{
private:
	CE::Camera *camera;
protected:
	ID3D11Texture2D *textureSrc;
	ID3D11Texture2D *textureDest;
	unsigned char *textureData;
	unsigned height;
	unsigned width;
	unsigned channel; // RGBSensor有4个Channel，DepthSensor有1个Channel
	float samplePeriod; // 采样率的倒数。采样率表示一秒中采样的次数。如果当前时间和上次采样的时间的间隔小于采样率的倒数，则不采样
	float lastSampleTime; // 上次采样的时间
	bool enabled;
	ICameraSensor(const ICameraSensor&) = delete;
	ICameraSensor& operator=(const ICameraSensor&) = delete;
	inline CE::Camera* GetCamera() const { return camera; }
	virtual unsigned char* GetBMPPixels() const = 0; // 根据textureData生成BMP的像素
public:
	ICameraSensor(CE::IComponent *comp, const char *cameraIdentifier, const char *param);
	virtual ~ICameraSensor();
	virtual const char *GetSensorType() const = 0;
	void PreRender(); // 在每次渲染前，如果enabled且camera的depth为负数，则把camera的depth设为0
	virtual bool Sample(); // 把Texture的每个Pixel从GPU复制到CPU，最后存入textureData
	inline bool IsEnabled() const { return enabled; }
	inline void SetEnabled(const bool enabled) { this->enabled = enabled; }
	inline const float *GetTextureData(unsigned &outHeight, unsigned &outWidth, unsigned &outChannel)
	{
		outHeight = height;
		outWidth = width;
		outChannel = channel;
		return reinterpret_cast<const float*>(textureData);
	}
	bool SaveBMP(const char *bmpFile) const; // 把当前的textureData保存为bmp图片（调试使用），保存规则由GetBMPPixels定义
};