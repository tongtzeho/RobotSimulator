#pragma once

#include "../Engine/CoolEngine.h"

#include "Communicator.h"

class CommunicationSystem : private CE::UnstableHashArray<Communicator*>
{
private:
	PyObject *pySend; // 通过communication_system的send函数进行消息接收者和消息处理
	PyObject *pyCommList;
	void UpdatePyCommunicatorList();
	PyObject *GetPyCommunicator(Communicator *sender) const;
public:
	CommunicationSystem();
	~CommunicationSystem();
	bool Append(Communicator *const communicator);
	bool Delete(Communicator *const communicator);
	void UpdatePositions();
	void Send(Communicator *sender, const char *msg);
};