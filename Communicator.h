#pragma once

#include <map>

#include "../Engine/CoolEngine.h"

class Communicator : private std::multimap<double, std::string>
{
private:
	const CE::IComponent *const component;
	CE::Vector3 position;
public:
	Communicator(const CE::IComponent *const comp) : component(comp) { UpdatePosition(); }
	~Communicator() = default;
	void UpdatePosition();
	void Send(const char *msg);
	void Receive(const char *msg, const double receiveTime) { insert(std::pair<double, std::string>(receiveTime, msg)); }
	void Read(std::vector<std::string> &outMessages);
	inline const CE::Vector3& GetPosition() const { return position; }
	inline const CE::IComponent* GetComponent() const { return component; }
};