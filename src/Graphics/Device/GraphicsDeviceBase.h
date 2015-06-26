
#pragma once 

#include <Lumino/Threading/Mutex.h>
#include "DeviceInterface.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

class GraphicsDeviceBase
	: public IGraphicsDevice
{
public:
	GraphicsDeviceBase();
	virtual ~GraphicsDeviceBase();

public:

	virtual void Finalize();

	/// ���\�[�X�o�^
	void AddDeviceResource(IDeviceObject* obj);

	/// ���\�[�X���O
	//void RemoveDeviceResource(IDeviceObject* obj);

	/// GC ���s
	void GCDeviceResource();

protected:
	typedef Array<IDeviceObject*>	DeviceObjectList;

	DeviceObjectList	m_deviceObjectList;
	Threading::Mutex	m_deviceObjectListMutex;
};

} // namespace Device
} // namespace Graphics
} // namespace Lumino
