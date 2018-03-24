#pragma once

#include "../Engine/CoolEngine.h"

class IActionController : private std::vector<float> // ��ά����
{
private:
	CE::IComponent *const component;
	IActionController(const IActionController&) = delete;
	IActionController& operator=(const IActionController&) = delete;
protected:
	IActionController(CE::IComponent *const comp, const size_t n) : component(comp), std::vector<float>(n, 0) {}
public:
	virtual ~IActionController() = default;
	inline CE::IComponent* GetComponent() const { return component; }
	virtual inline const char *GetActionControllerName() const = 0; // ���ؾ�����ж����������ƣ���"EpuckActionController"��
	virtual bool IsOutOfControl() const = 0;
	virtual bool IsCrashed() const = 0;
	virtual void Action(const float dt) const = 0; // ���ж�״̬�����ʩ�ӵ�Entity��Entity������Component��
	inline float GetActionState(const size_t n) const { return n < size() ? (*this)[n] : 0; }
	inline const std::vector<float>& GetActionState() const { return (*this); }
	inline void SetActionState(const size_t n, const float state) { if (n < size()) { (*this)[n] = state; } }
	void SetActionState(const std::vector<float> &states)
	{
		for (size_t i = 0; i < states.size() && i < size(); ++i)
		{
			(*this)[i] = states[i];
		}
	}
};