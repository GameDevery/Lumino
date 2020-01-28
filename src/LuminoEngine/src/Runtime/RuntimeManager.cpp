﻿#include "RuntimeManager.hpp"

namespace ln {
namespace detail {
    
//==============================================================================
// RuntimeManager

RuntimeManager::Settings RuntimeManager::s_globalSettings;

//LnReferenceCountTrackerCallback RuntimeManager::m_referenceCountTracker = nullptr;
//LnRuntimeFinalizedCallback RuntimeManager::m_runtimeFinalizedCallback = nullptr;

RuntimeManager::RuntimeManager()
	: m_systemAliving(false)
{
    // 特に Binding-module にて、Engine 初期化以前にいろいろ処理を行うが、
    // そこのログを出力したいので他の設定より先に有効化しておく。
    ln::GlobalLogger::addStdErrAdapter();
#ifdef LN_OS_ANDROID
    ln::GlobalLogger::addLogcatAdapter();
#endif
#if defined(LN_OS_MAC) || defined(LN_OS_IOS)
    ln::GlobalLogger::addNLogAdapter();
#endif
}

RuntimeManager::~RuntimeManager()
{
}

void RuntimeManager::init(const Settings& settings)
{
	LN_LOG_DEBUG << "RuntimeManager Initialization started.";

    m_settings = settings;

	// オブジェクト管理配列
	for (int i = 511; i >= 0; --i)
	{
		ObjectEntry e;
		e.object = nullptr;
		e.index = i;
		m_objectEntryList.add(e);

		if (i > 0) {  // 0 は NULL 扱い。欠番にする。
			m_objectIndexStack.push(i);
		}
	}

	m_systemAliving = true;

	LN_LOG_DEBUG << "RuntimeManager Initialization finished.";
}

void RuntimeManager::dispose()
{
	LN_LOG_DEBUG << "RuntimeManager finalization started.";

	for (auto& e : m_objectEntryList) {
		if (e.object) {
			auto runtimeData = detail::ObjectHelper::getRuntimeData(e.object);
			if (runtimeData->fromCreate) {
				RefObjectHelper::release(e.object);	// If the reference count reaches 0, onDestructObject is called.
			}
		}
	}

	m_systemAliving = false;
    m_settings.referenceCountTrackerCallback = nullptr;

    if (m_settings.runtimeFinalizedCallback) {
        m_settings.runtimeFinalizedCallback();
        m_settings.runtimeFinalizedCallback = nullptr;
    }

	LN_LOG_DEBUG << "RuntimeManager finalization finished.";
}

LnHandle RuntimeManager::makeObjectWrap(Object* obj, bool fromCreate)
{
	if (!m_systemAliving) return LN_NULL_HANDLE;
	if (obj == nullptr) return LN_NULL_HANDLE;

	if (fromCreate) {
		RefObjectHelper::retain(obj);
	}

	// 登録済みならハンドル (管理配列上のインデックス) を返す
	auto runtimeData = detail::ObjectHelper::getRuntimeData(obj);
	if (runtimeData)
	{
		return runtimeData->index;
	}

	// 管理配列がすべて埋まっている場合
	if (m_objectIndexStack.empty())
	{
		// 末尾に追加する
		ObjectEntry e;
		e.object = obj;
		e.index = m_objectEntryList.size();
		e.refCount = (fromCreate) ? 1 : 0;
		m_objectEntryList.add(e);

		// register runtime data
		auto data = LN_NEW ObjectRuntimeData();
		data->index = e.index;
		data->fromCreate = fromCreate;
		detail::ObjectHelper::setRuntimeData(obj, data);

		return e.index;
	}
	else
	{
		// 空き場所を取得
		int newPos = m_objectIndexStack.top();
		m_objectIndexStack.pop();

		// 格納
		ObjectEntry& e = m_objectEntryList[newPos];
		e.object = obj;
		e.index = newPos;
		e.refCount = (fromCreate) ? 1 : 0;

		// register runtime data
		auto data = LN_NEW ObjectRuntimeData();
		data->index = e.index;
		data->fromCreate = fromCreate;
		detail::ObjectHelper::setRuntimeData(obj, data);

		return e.index;
	}
}

void RuntimeManager::retainObjectExplicitly(LnHandle handle)
{
	auto& e = m_objectEntryList[static_cast<int>(handle)];
	RefObjectHelper::retain(e.object);
#if 0
	auto& e = m_objectEntryList[static_cast<int>(handle)];
	auto* runtimeData = detail::ObjectHelper::getRuntimeData(e.object);
	
	if (!runtimeData->fromCreate && e.refCount == 0) {
		// generated by "Get", retain native object.
		runtimeData->fromCreate = true;
		e.refCount = 1;
		RefObjectHelper::retain(e.object);
	}
	else if (runtimeData->fromCreate && e.refCount > 0) {
		// generated by "Create"
		e.refCount++;
	}
	else {
		LN_UNREACHABLE();
	}
#endif
}

void RuntimeManager::releaseObjectExplicitly(LnHandle handle)
{
	if (!m_systemAliving) return;

	int index = static_cast<int>(handle);
	ObjectEntry& e = m_objectEntryList[index];


	// 明示的に release したので自動 release は不要
	auto runtimeData = detail::ObjectHelper::getRuntimeData(e.object);
	runtimeData->fromCreate = false;


	RefObjectHelper::release(e.object);		// If the reference count reaches 0, onDestructObject is called.

	//if (e.object)
	//{
	//	e.externalRefCount--;
	//	if (e.externalRefCount <= 0)
	//	{
	//		RefObjectHelper::release(e.object);
	//		m_objectIndexStack.push(index);
	//	}
	//}
}

void RuntimeManager::onDestructObject(Object* obj)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto runtimeData = detail::ObjectHelper::getRuntimeData(obj);
	if (runtimeData) {
		ObjectEntry& e = m_objectEntryList[runtimeData->index];
		LN_CHECK(e.object == obj);

		e.refCount--;
		if (e.refCount <= 0) {
			m_objectIndexStack.push(runtimeData->index);
			e.object = nullptr;
		}

		delete runtimeData;
	}
}

ObjectEntry* RuntimeManager::getObjectEntry(LnHandle handle)
{
	return &m_objectEntryList[static_cast<int>(handle)];
}

Object* RuntimeManager::getObjectFromHandle(LnHandle handle)
{
    return m_objectEntryList[static_cast<int>(handle)].object;
}

void RuntimeManager::setManagedObjectId(LnHandle handle, int64_t id)
{
	auto runtimeData = detail::ObjectHelper::getRuntimeData(m_objectEntryList[static_cast<int>(handle)].object);
	runtimeData->managedObjectId = id;
}

int64_t RuntimeManager::getManagedObjectId(LnHandle handle)
{
	auto runtimeData = detail::ObjectHelper::getRuntimeData(m_objectEntryList[static_cast<int>(handle)].object);
	return runtimeData->managedObjectId;
}

int64_t RuntimeManager::getManagedTypeInfoId(LnHandle handle)
{
	auto obj = m_objectEntryList[static_cast<int>(handle)].object;
	auto typeInfo = TypeInfo::getTypeInfo(obj);
	return detail::TypeInfoInternal::getManagedTypeInfoId(typeInfo);
}

//void RuntimeManager::setReferenceCountTracker(LnReferenceCountTrackerCallback callback)
//{
//	m_referenceCountTracker = callback;
//}
//
//void RuntimeManager::setRuntimeFinalizedCallback(LnRuntimeFinalizedCallback callback)
//{
//    m_runtimeFinalizedCallback = callback;
//}

//void RuntimeManager::setRuntimeCreateInstanceCallback(LnRuntimeCreateInstanceCallback callback)
//{
//    LN_NOTIMPLEMENTED();
//}
//
//void RuntimeManager::setRuntimeGetTypeInfoIdCallback(LnRuntimeGetTypeInfoIdCallback callback)
//{
//    LN_NOTIMPLEMENTED();
//}

void RuntimeManager::setReferenceTrackEnabled(LnHandle handle)
{
	RefObjectInternal::setObjectFlag(m_objectEntryList[static_cast<int>(handle)].object, RefObjectFlags_ReferenceTracking, true);
}

void RuntimeManager::onRetainedObject(Object* obj)
{
	if (m_settings.referenceCountTrackerCallback) {
		if (auto runtimeData = detail::ObjectHelper::getRuntimeData(obj)) {
            m_settings.referenceCountTrackerCallback(runtimeData->index, LNI_REFERENCE_RETAINED, RefObjectHelper::getReferenceCount(obj));
		}
	}
}

void RuntimeManager::onReleasedObject(Object* obj)
{
	if (m_settings.referenceCountTrackerCallback) {
		if (auto runtimeData = detail::ObjectHelper::getRuntimeData(obj)) {
            m_settings.referenceCountTrackerCallback(runtimeData->index, LNI_REFERENCE_RELEASED, RefObjectHelper::getReferenceCount(obj));
		}
	}
}

LnResult RuntimeManager::processException(Exception* e)
{
	return LN_ERROR_UNKNOWN;
}

} // namespace detail
} // namespace ln
