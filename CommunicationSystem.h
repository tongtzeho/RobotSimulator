#pragma once

#include "../Engine/CoolEngine.h"

#include "Communicator.h"

class CommunicationSystem : private CE::UnstableHashArray<Communicator*>
{
private:
	PyObject *pySend; // ͨ��communication_system��send����������Ϣ�����ߺ���Ϣ����
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