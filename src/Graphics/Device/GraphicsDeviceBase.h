﻿
#pragma once 

#include <Lumino/Threading/Mutex.h>
#include "DeviceInterface.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
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

	/// リソース登録
	void AddDeviceResource(IDeviceObject* obj);

	/// リソース除外
	//void RemoveDeviceResource(IDeviceObject* obj);

	/// GC 実行
	void GCDeviceResource();

protected:
	typedef Array<IDeviceObject*>	DeviceObjectList;

	DeviceObjectList	m_deviceObjectList;
	Threading::Mutex	m_deviceObjectListMutex;
};

} // namespace Device
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
